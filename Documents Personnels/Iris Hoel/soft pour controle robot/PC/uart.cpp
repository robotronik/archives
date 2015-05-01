/*=============================================================================
 *
 *
 *          Control du robot par stick et visualisation
 *               de la cmd et du retour des moteurs
 *
 *
 *===========================================================================*/


/*------------------------------- include -----------------------------------*/


#include "uart.hpp"



/*------------------------------- variables ---------------------------------*/


HANDLE hComm = NULL;
OVERLAPPED Oo;
SDL_TimerID timerTestAsserAlive;
SDL_TimerID timeOutBetweenCodeAndData;
static int writeEnable = 1;
static int codeAck = 0;
static int dataAck = 0;
static int modeData = 0;
static unsigned int saveLength = 0;


/*------------------------------- declaration fonctions ---------------------*/



/*-------------------- com uart high level --------------*/

static void RX_end_of_packet_handler(unsigned char* frame);
static void sendCode(unsigned char addr, unsigned int code);
static void sendData(unsigned char addr, int data);
static void retryCode(unsigned char addr, unsigned int code);
static void retryData(unsigned char addr, int data);
Uint32 timeOutBetweenCodeAndDataHandler(Uint32 intervalle, void*);
Uint32 timerTestAsser(Uint32 intervalle, void*);
static void clearRX(void);
static int checkCode(unsigned int code, unsigned char *addr);


/*-------------------- com uart medium level ------------*/

static void send(unsigned char addr, int data);











/*---------------------------------------------------------------------------*/





/*------------------------------ com uart high level ------------------------*/


/* functions like as PSOC one less read call and time out*/

void write_assert(unsigned char addr, unsigned int code, int data)
{
    if(writeEnable && (modeData == 0))
    {
        clearRX();
        sendCode(addr, code);
        if(writeEnable)
            sendData(addr, data);
    }
    else
    {
        #ifdef UART_DEBUG
        printf("PC uart write ignored\n");
        #endif
    }
}

static void sendCode(unsigned char addr, unsigned int code)
{
    send(addr, code);

    int tmp = SDL_GetTicks();
    while(!codeAck)
    {
        SDL_Delay(DELAY_UART_LOOP);
        if(SDL_GetTicks() - tmp > ACK_TIME_OUT)
        {
            retryCode(addr, code);
            return;
        }
        readUart(hComm, &Oo);
    }
    codeAck = 0;
}

static void sendData(unsigned char addr, int data)
{
    send(addr, data);
    int tmp = SDL_GetTicks();
    while(!dataAck)
    {
        SDL_Delay(DELAY_UART_LOOP);
        if(SDL_GetTicks() - tmp > ACK_TIME_OUT)
        {
            retryData(addr, data);
            return;
        }
        readUart(hComm, &Oo);
    }
    dataAck = 0;
}

static void retryCode(unsigned char addr, unsigned int code)
{
    static unsigned int nbLoop = 0;
    #ifdef UART_ERROR
    printf("PC retry code in %dms\n",DELAY_BEFORE_RETRY );
    #endif
    int tmp = SDL_GetTicks();

    if(nbLoop < NB_RETRY)
    {
        nbLoop ++;
        while(SDL_GetTicks() - tmp < DELAY_BEFORE_RETRY)
        {
            SDL_Delay(DELAY_UART_LOOP);
            readUart(hComm, &Oo);
            clearRX();
        }

        sendCode(addr, code);
        nbLoop = 0;
    }
    else
    {
        #ifdef UART_ERROR
        printf("PC asser didn't respond, cmd ignore during the next %dms\n", TIME_CHECK_ASSERT);
        #endif
        nbLoop = 0;
        writeEnable = 0;
        timerTestAsserAlive = SDL_AddTimer(TIME_CHECK_ASSERT, timerTestAsser,0);
    }
}

static void retryData(unsigned char addr, int data)
{
    static unsigned int nbLoop = 0;
    #ifdef UART_ERROR
    printf("PC retry data in %dms\n",DELAY_BEFORE_RETRY );
    #endif
    int tmp = SDL_GetTicks();

    if(nbLoop < NB_RETRY)
    {
        nbLoop ++;
        while(SDL_GetTicks() - tmp < DELAY_BEFORE_RETRY)
        {
            SDL_Delay(DELAY_UART_LOOP);
            readUart(hComm, &Oo);
            clearRX();
        }

        sendData(addr, data);
        nbLoop = 0;
    }
    else
    {
        #ifdef UART_ERROR
        printf("PC asser didn't respond, cmd ignore during the next %dms\n", TIME_CHECK_ASSERT);
        #endif
        nbLoop = 0;
        writeEnable = 0;
        timerTestAsserAlive = SDL_AddTimer(TIME_CHECK_ASSERT, timerTestAsser,0);
    }
}

static void RX_end_of_packet_handler(unsigned char* frame)
{
    unsigned int data = 0;
    static unsigned int codeRead = 0;
    static unsigned char addr = 0;
    writeEnable = 1;
    SDL_RemoveTimer(timerTestAsserAlive);

    #ifdef UART_DEBUG
    printf("PC received    <- %2x%3x%3x%3x%3x   at%9d\n", frame[0],frame[1],frame[2],frame[3],frame[4], SDL_GetTicks());
    #endif

    switch(frame[0])
    {
        case ADRESSE_PSOC:
            return;
        case ADRESSE_ASSER:
            return;
        case ADRESSE_PC:
            data = (((unsigned int)frame[1]) << 24)
                    | (((unsigned int)frame[2]) << 16)
                    | (((unsigned int)frame[3]) << 8)
                    | ((unsigned int)frame[4]);
            if(data == 0x7FFFFFFF)
            {
                if(codeAck == 1)
                {
                    #ifdef UART_ERROR
                    printf("PC overflow error\n");
                    #endif
                    clearRX();
                }
                else
                {
                    codeAck = 1;
                }
            }
            else if(data == 0x3FFFFFFF)
            {
                if(dataAck == 1)
                {
                    #ifdef UART_ERROR
                    printf("PC overflow error\n");
                    #endif
                    clearRX();
                }
                else
                    dataAck = 1;
            }
            else
            {
                if(!modeData)
                {
                    if(checkCode(data, &addr))
                    {
                        codeRead = data;
                        modeData = 1;
                        send(addr, 0x7FFFFFFF);
                        timeOutBetweenCodeAndData = SDL_AddTimer(TMP_OUT_BETWEEN_ACK_AND_DATA,
                                                        timeOutBetweenCodeAndDataHandler, 0);
                    }
                    else
                    {
                        #ifdef UART_ERROR
                        printf("PC code read error\n");
                        #endif
                        clearRX();
                    }
                }
                else
                {
                    modeData = 0;
                    send(addr, 0x3FFFFFFF);
                    SDL_RemoveTimer(timeOutBetweenCodeAndData);
                    process(codeRead, (int)data);
                }
            }
            break;
        default:
            #ifdef UART_ERROR
            printf("PC addr error\n");
            #endif
            clearRX();
            return;
    }
}


Uint32 timeOutBetweenCodeAndDataHandler(Uint32 intervalle, void*)
{
    #ifdef UART_ERROR
    printf("PC time out between code and data error\n");
    #endif
    clearRX();

    return intervalle;
}

Uint32 timerTestAsser(Uint32 intervalle, void* )
{
    writeEnable = 1;
    SDL_RemoveTimer(timerTestAsserAlive);
    return intervalle;
}

static void clearRX(void)
{
    readUart(hComm, &Oo);
    SDL_RemoveTimer(timeOutBetweenCodeAndData);
    codeAck = 0;
    dataAck = 0;
    modeData = 0;
    saveLength = 0;
}


static int checkCode(unsigned int code, unsigned char *addr)
{
    if(code == MOTOR)
    {
        *addr = ADRESSE_ASSER;
        return 1;
    }
    return 0;
}


/*------------------------------ com uart medium level ------------------------*/



static void send(unsigned char addr, int data)
{
    char tab[5] = {(char) addr, (char) (data >> 24), (char) (data >> 16), (char) (data >> 8), (char) (data)};
    #ifdef UART_DEBUG
    printf("PC send        -> %2x %2x %2x %2x %2x   at%9d\n", tab[0],(unsigned char) tab[1], 
                        (unsigned char) tab[2],(unsigned char) tab[3],(unsigned char) tab[4], SDL_GetTicks());
    #endif

    WriteABuffer(tab, 5, hComm, &Oo);
    //printf("send finish at %9d\n", SDL_GetTicks());
}



/*------------------------------ com uart low level --------------------------*/

OVERLAPPED initCom() {
    // Create the overlapped event. Must be closed before exiting
    // to avoid a handle leak.
    OVERLAPPED osReader = {0};
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (osReader.hEvent == NULL) {
    }
    return osReader;
}

HANDLE link(int conf) {
    DCB dcb;
    COMMTIMEOUTS timeouts;

    HANDLE hComm = CreateFile( "COM3",
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL);
    //options[4] = _T(' ');
    if (hComm == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    FillMemory(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(dcb);

    if(conf == CONF_UART_9600_BAUD)
    {
        if (!BuildCommDCB("baud=9600 parity=N data=8 stop=1", &dcb)) {
            CloseHandle(hComm);
            return NULL;
        }
    }
    else
    {
        if (!BuildCommDCB("baud=115200 parity=N data=8 stop=1", &dcb)) {
            CloseHandle(hComm);
            return NULL;
        }
    }

        // Set new state.
    if (!SetCommState(hComm, &dcb)) {
        CloseHandle(hComm);
        return NULL;
    }

    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;

    if (!SetCommTimeouts(hComm, &timeouts)) {
        CloseHandle(hComm);
        return NULL;
    }

    // SetTimer(hWnd,              // handle to main window
    //     IDT_TIMER,              // timer identifier
    //     100,                    // millisecond interval
    //     (TIMERPROC) NULL);      // no timer callback

    return hComm;
}

void WriteABuffer(char *lpBuf, DWORD dwToWrite, HANDLE hComm, OVERLAPPED *osReader)
{
    OVERLAPPED osWrite = {0};
    DWORD dwWritten, dwRes;

    // Create this writes OVERLAPPED structure hEvent.
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osWrite.hEvent == NULL) {
        return;
    }

    //read(hComm, osReader);

    // Issue write.
    if (!WriteFile(hComm, lpBuf, dwToWrite, &dwWritten, &osWrite))
    {
        if (GetLastError() != ERROR_IO_PENDING)
        {
        }
        else {
            // Write is pending.
            dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
            switch(dwRes)
            {
                // OVERLAPPED structure's event has been signaled.
                case WAIT_OBJECT_0:
                    GetOverlappedResult(hComm, &osWrite, &dwWritten, FALSE);
                    //exit_soft();
                    break;

                default:
                    // This usually indicates a problem with the
                    // OVERLAPPED structure's event handle.
                    break;
            }
        }
    }
    CloseHandle(osWrite.hEvent);
}

void readUart(HANDLE hComm, OVERLAPPED *osReader) {
    unsigned char pRxBuffer[100] = {0,};
    static unsigned char save[5] = {0,};
    static int let_printf_mode = 0;
    static int end_of_printf = 0;

    DWORD dwRead, i;

    // Issue read operation.
    if (!ReadFile(hComm, pRxBuffer, 100, &dwRead, osReader)) {
        if (GetLastError() != ERROR_IO_PENDING) {    // read not delayed?
        }
    } else if (dwRead) {
        // read completed immediately
        unsigned int rxIndice = 0;
        unsigned int rxLength = dwRead;
        while(rxLength)
        {
            if(let_printf_mode)
            {
                //char carac = pRxBuffer[rxIndice];
                if(pRxBuffer[rxIndice] == 0x0D)
                    end_of_printf = 1;
                else if(end_of_printf)
                {
                    if(pRxBuffer[rxIndice] == 0x0A)
                    {
                        let_printf_mode = 0;
                        // #ifdef UART_DEBUG
                        // printf("\nend of external printf\n");
                        // #endif
                    }
                        end_of_printf == 0;
                }
                printf("%c", pRxBuffer[rxIndice]);
                rxIndice ++;
                rxLength --;
            }
            else if(rxLength + saveLength < 5)
            {
                save[saveLength] = pRxBuffer[rxIndice];
                #ifdef UART_READ_BRUT
                printf("read brut %x\n",pRxBuffer[rxIndice]);
                #endif
                saveLength ++;
                rxIndice ++;
                rxLength --;
            }
            else
            {
                while(saveLength < 5)
                {
                    save[saveLength] = pRxBuffer[rxIndice];
                    #ifdef UART_READ_BRUT
                    printf("read brut %x\n",pRxBuffer[rxIndice]);
                    #endif
                    saveLength ++;
                    rxIndice ++;
                    rxLength --;
                }

                if((save[0] == 0x41 && save[1] == 0x73 && save[2] == 0x73 && save[3] == 0x65) ||
                    (save[0] == 0x50 && save[1] == 0x53 && save[2] == 0x4F && save[3] == 0x43))
                {
                    /* it's a printf */
                    let_printf_mode = 1;
                    // #ifdef UART_DEBUG
                    // printf("start of external printf\n");
                    // #endif
                    printf("%5s", save);
                }
                else
                    RX_end_of_packet_handler(save);

                saveLength = 0;
            }
        }
    }
}



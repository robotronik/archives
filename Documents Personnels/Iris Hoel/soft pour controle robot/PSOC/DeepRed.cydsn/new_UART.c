/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/



#include "new_UART.h"


/*------------------------------- variables ---------------------------------*/


static int timerTestAsserAlive;
static int timeOutBetweenCodeAndData;

static int writeEnable = 1;
static int codeAck = 0;
static int dataAck = 0;
static int modeData = 0;

static unsigned int saveLengthAsser = 0;
static unsigned int saveLengthXBee = 0;

static unsigned int count = 0;

/*------------------------------- declaration fonctions ---------------------*/



/*-------------------- com uart high level --------------*/

static void process(unsigned int code, int data);
static void sendCode(unsigned char addr, unsigned int code);
static void sendData(unsigned char addr, int data);
static void retryCode(unsigned char addr, unsigned int code);
static void retryData(unsigned char addr, int data);
static void RX_end_of_packet_handler(unsigned char* frame);


void timeOutBetweenCodeAndDataHandler(void);
void timerTestAsser(void);
static void clearRX(void);
static int checkCode(unsigned int code, unsigned char *addr);


/*-------------------- com uart medium level ------------*/

static void send(unsigned char addr, int data);


/*------------------------------ timer ------------------*/

static void initTimer(void);
static void stopTimer(void);
static int AddTimer(unsigned int period, void (*callback)(void));
static void RemoveTimer(int ID_Timer);












/*---------------------------------------------------------------------------*/





/*------------------------------ com uart high level ------------------------*/

static void process(unsigned int code, int data)
{
    if(code == 0x35)
    {
        printf("PSOC             time %d\n", count);
    }
    // if(code == MOTOR)
    // {
    //     //printf("bouh, %9d\n", SDL_GetTicks());
    //     leftReel = (int)(data >> 16);
    //     rightReel = (int)data;
    // }
}


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
        printf("PSOC uart write ignored\n");
        #endif
    }
}

static void sendCode(unsigned char addr, unsigned int code)
{
    send(addr, code);

    int tmp = count;
    while(!codeAck)
    {
        //SDL_Delay(DELAY_UART_LOOP);
        if(count - tmp > ACK_TIME_OUT)
        {
            retryCode(addr, code);
            return;
        }
        //readUart(hComm, &Oo);
    }
    codeAck = 0;
}

static void sendData(unsigned char addr, int data)
{
    send(addr, data);
    int tmp = count;
    while(!dataAck)
    {
        //SDL_Delay(DELAY_UART_LOOP);
        if(count - tmp > ACK_TIME_OUT)
        {
            retryData(addr, data);
            return;
        }
        //readUart(hComm, &Oo);
    }
    dataAck = 0;
}

static void retryCode(unsigned char addr, unsigned int code)
{
    static unsigned int nbLoop = 0;
    #ifdef UART_ERROR
    printf("PSOC retry code in %dms\n",DELAY_BEFORE_RETRY );
    #endif
    int tmp = count;

    if(nbLoop < NB_RETRY)
    {
        nbLoop ++;
        while(count - tmp < DELAY_BEFORE_RETRY)
        {
            //SDL_Delay(DELAY_UART_LOOP);
            //readUart(hComm, &Oo);
            clearRX();
        }

        sendCode(addr, code);
        nbLoop = 0;
    }
    else
    {
        #ifdef UART_ERROR
        printf("PSOC asser didn't respond, cmd ignore during the next %dms\n", TIME_CHECK_ASSERT);
        #endif
        nbLoop = 0;
        writeEnable = 0;
        timerTestAsserAlive = AddTimer(TIME_CHECK_ASSERT, timerTestAsser);
    }
}

static void retryData(unsigned char addr, int data)
{
    static unsigned int nbLoop = 0;
    #ifdef UART_ERROR
    printf("PSOC retry data in %dms\n",DELAY_BEFORE_RETRY );
    #endif
    int tmp = count;

    if(nbLoop < NB_RETRY)
    {
        nbLoop ++;
        while(count - tmp < DELAY_BEFORE_RETRY)
        {
            //SDL_Delay(DELAY_UART_LOOP);
            //readUart(hComm, &Oo);
            clearRX();
        }

        sendData(addr, data);
        nbLoop = 0;
    }
    else
    {
        #ifdef UART_ERROR
        printf("PSOC asser didn't respond, cmd ignore during the next %dms\n", TIME_CHECK_ASSERT);
        #endif
        nbLoop = 0;
        writeEnable = 0;
        timerTestAsserAlive = AddTimer(TIME_CHECK_ASSERT, timerTestAsser);
    }
}


static void RX_end_of_packet_handler(unsigned char* frame)
{
    unsigned int data = 0;
    static unsigned int codeRead = 0;
    static unsigned char addr = 0;
    writeEnable = 1;
    RemoveTimer(timerTestAsserAlive);

    //#ifdef UART_DEBUG
    printf("PSOC received  <- %2x%3x%3x%3x%3x\n", frame[0],frame[1],frame[2],frame[3],frame[4]);
    //#endif

    switch(frame[0])
    {
        case ADRESSE_PC:
            #ifdef UART_DEBUG
            printf("PSOC addr to PC\n");
            #endif
            UART_XBee_PutChar(frame[0]);
            UART_XBee_PutChar(frame[1]);
            UART_XBee_PutChar(frame[2]);
            UART_XBee_PutChar(frame[3]);
            UART_XBee_PutChar(frame[4]);
            return;
        case ADRESSE_ASSER:
            #ifdef UART_DEBUG
            printf("PSOC addr to ASSER\n");
            #endif
            UART_Asser_PutChar(frame[0]);
            UART_Asser_PutChar(frame[1]);
            UART_Asser_PutChar(frame[2]);
            UART_Asser_PutChar(frame[3]);
            UART_Asser_PutChar(frame[4]);
            return;
        case ADRESSE_PSOC:
            data = (((unsigned int)frame[1]) << 24)
                    | (((unsigned int)frame[2]) << 16)
                    | (((unsigned int)frame[3]) << 8)
                    | ((unsigned int)frame[4]);
            if(data == 0x7FFFFFFF)
            {
                if(codeAck == 1)
                {
                    #ifdef UART_ERROR
                    printf("PSOC : overflow error\n");
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
                    printf("PSOC : overflow error\n");
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
                        // timeOutBetweenCodeAndData = AddTimer(TMP_OUT_BETWEEN_ACK_AND_DATA,
                        //                                 timeOutBetweenCodeAndDataHandler);
                    }
                    else
                    {
                        #ifdef UART_ERROR
                        printf("PSOC : code read error\n");
                        #endif
                        clearRX();
                    }
                }
                else
                {
                    modeData = 0;
                    send(addr, 0x3FFFFFFF);
                    // RemoveTimer(timeOutBetweenCodeAndData);
                    process(codeRead, (int)data);
                }
            }
            break;
        default:
            #ifdef UART_ERROR
            printf("PSOC : addr error\n");
            #endif
            clearRX();
            return;
    }
}


void timeOutBetweenCodeAndDataHandler(void)
{
    #ifdef UART_ERROR
    printf("PSOC : time out between code and data error\n");
    #endif
    clearRX();
}

void timerTestAsser(void)
{
    writeEnable = 1;
    RemoveTimer(timerTestAsserAlive);
}

static void clearRX(void)
{
    RemoveTimer(timeOutBetweenCodeAndData);
    saveLengthAsser = 0;
    saveLengthXBee = 0;
	int i ;
    for(i=0;i<10;i++)
    {
        UART_XBee_GetChar();
        UART_Asser_GetChar();
    }

    // UART_rxBufferRead = UART_rxBuffer;
    //UART_XBee_Init();
    //UART_Asser_Init();
    // UART_Asser_ClearRxBuffer();
    // UART_Asser_ClearTxBuffer();
    // UART_XBee_ClearRxBuffer();
    // UART_XBee_ClearTxBuffer();
    codeAck = 0;
    dataAck = 0;
    modeData = 0;
}


static int checkCode(unsigned int code, unsigned char *addr)
{
    if(code == 0x35)
    {
        *addr = ADRESSE_PC;
        return 1;
    }
    // if(code == MOTOR)
    // {
    //     *addr = ADRESSE_ASSER;
    //     return 1;
    // }
    return 0;
}


/*------------------------------ com uart medium level ------------------------*/



static void send(unsigned char addr, int data)
{
    #ifdef UART_DEBUG
    //printf("PSOC send     -> %2x %8x    \n", addr, data);
    #endif

    if(addr == ADRESSE_ASSER)
    {
        UART_Asser_PutChar((char) addr);
        UART_Asser_PutChar((char) (data >> 24));
        UART_Asser_PutChar((char) (data >> 16));
        UART_Asser_PutChar((char) (data >> 8));
        UART_Asser_PutChar((char) (data));        
    }
    else
    {
        UART_XBee_PutChar((char) addr);
        UART_XBee_PutChar((char) (data >> 24));
        UART_XBee_PutChar((char) (data >> 16));
        UART_XBee_PutChar((char) (data >> 8));
        UART_XBee_PutChar((char) (data));        
    }
}


/*------------------------------ com uart low level --------------------------*/


void init_com(void)
{
    UART_XBee_Start();
    isr_UART_XBee_Rx_Start();
    UART_Asser_Start();
    isr_UART_Asser_Rx_Start(); 

    initTimer();
}

void close_com(void)
{
    stopTimer();

    isr_UART_XBee_Rx_Start();
    UART_XBee_Start();
    isr_UART_Asser_Rx_Start(); 
    UART_Asser_Start();
}



void RX_ASSER_IT_Handler(void)
{
    static unsigned char save[5] = {0,};
    static int let_printf_mode = 0;
    static int end_of_printf = 0;

    // read completed immediately
    unsigned int rxLength = UART_Asser_GetRxBufferSize();
    while(rxLength)
    {
        if(let_printf_mode)
        {
            char carac = UART_Asser_GetChar();
            if(carac == 0x0D)
                end_of_printf = 1;
            else if(end_of_printf)
            {
                if(carac == 0x0A)
                {
                    let_printf_mode = 0;
                    #ifdef UART_DEBUG
                    //printf("PSOC : end of external printf\n");
                    #endif
                }
                end_of_printf == 0;
            }
            UART_XBee_PutChar(carac);
            rxLength --;
        }
        else if(rxLength + saveLengthAsser < 5)
        {
            save[saveLengthAsser] = UART_Asser_GetChar();
            #ifdef UART_READ_BRUT
            printf("PSOC : read brut %x\n",save[saveLengthAsser]);
            #endif
            saveLengthAsser ++;
            rxLength --;
        }
        else
        {
            while(saveLengthAsser < 5)
            {
                save[saveLengthAsser] = UART_Asser_GetChar();
                #ifdef UART_READ_BRUT
                printf("PSOC : read brut %x\n",save[saveLengthAsser]);
                #endif
                saveLengthAsser ++;
                rxLength --;
            }

            if(save[0] == 0x41 && save[1] == 0x73 && save[2] == 0x73 && save[3] == 0x65)
            {
                /* it's a printf */
                let_printf_mode = 1;
                #ifdef UART_DEBUG
                //printf("PSOC : start of external printf\n");
                #endif
                UART_XBee_PutChar(save[0]);
                UART_XBee_PutChar(save[1]);
                UART_XBee_PutChar(save[2]);
                UART_XBee_PutChar(save[3]);
                UART_XBee_PutChar(save[4]);
            }
            else
                RX_end_of_packet_handler(save);

            saveLengthAsser = 0;
        }
    }
}    



void RX_XBEE_IT_Handler(void)
{
    static unsigned char save[5] = {0,};
    static unsigned int saveLengthXBee = 0;

    // read completed immediately
    unsigned int rxLength = UART_XBee_GetRxBufferSize();
    while(rxLength)
    {
        if(rxLength + saveLengthXBee < 5)
        {
            save[saveLengthXBee] = UART_XBee_GetChar();
            #ifdef UART_READ_BRUT
            printf("PSOC : read brut %x\n",save[saveLengthXBee]);
            #endif
            saveLengthXBee ++;
            rxLength --;
        }
        else
        {
            while(saveLengthXBee < 5)
            {
                save[saveLengthXBee] = UART_XBee_GetChar();
                #ifdef UART_READ_BRUT
                printf("PSOC : read brut %x\n",save[saveLengthXBee]);
                #endif
                saveLengthXBee ++;
                rxLength --;
            }

            RX_end_of_packet_handler(save);

            saveLengthXBee = 0;
        }
    }
}  





/*------------------------------ timer --------------------------------------*/


typedef struct Timer
{
    int ID;
    unsigned int period;
    unsigned int nextRise;
    void (*callback)(void);
}Timer;


static Timer tabTimer[2];


static void initTimer(void)
{
    tabTimer[0].ID = 0;
    tabTimer[1].ID = 0;
    Timer_ms_Start();
    /* IT on timer_ms increment : */
    isr_systick_Start();
}

static void stopTimer(void)
{
    RemoveTimer(timerTestAsserAlive);
    RemoveTimer(timeOutBetweenCodeAndData);
    stopTimer();
    isr_systick_Stop();
    Timer_ms_Stop();
}


static int AddTimer(unsigned int period, void (*callback)(void))
{
    int indice = 0;
    
    if(tabTimer[0].ID == 0)
        indice = 0;
    else
        indice = 1;
    
    tabTimer[indice].ID = indice+1;
    tabTimer[indice].period = period;
    tabTimer[indice].nextRise = period + count;
    tabTimer[indice].callback = callback;

    return indice+1;
}


static void RemoveTimer(int ID_Timer)
{

    tabTimer[ID_Timer-1].ID = 0;    
}



void Systick_IT_Handler(void)
{
	count ++;
    int i = 0;
    for(i = 0; i < 2; i ++)
    {
        if((tabTimer[i].ID != 0) && (tabTimer[i].nextRise <=  count))
        {
            tabTimer[i].callback();
            tabTimer[i].nextRise = tabTimer[i].period + count;
        }
    }
}

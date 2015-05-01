#include "UART.h"

OVERLAPPED initCom();
HANDLE link();
void WriteABuffer(char *lpBuf, DWORD dwToWrite, HANDLE hComm, OVERLAPPED *osReader);
void readUart(HANDLE hComm, OVERLAPPED *osReader);


XBee::XBee() : uartInfos(UARTInit())
{}

XBee::~XBee()
{
    UARTClose(uartInfos);
}

void XBee::putChar(unsigned char c)
{
    UARTPutChar(uartInfos, c);
    SDL_Delay(30);
}

void XBee::sendPWMGauche(char value)
{
    static char lastValue = 0;
    if(value == lastValue)
        return;
    putChar(value & 0b01111111);
    lastValue = value;
}

void XBee::sendPWMDroit(char value)
{
    static char lastValue = 0;
    if(value == lastValue)
        return;
    putChar(value | 0b10000000);
    lastValue = value;
}

void XBee::sendVitAlpha(float vit, float alpha, bool nitroG, bool nitroD)
{
    float vitG, vitD;
    if(alpha >= 0)
    {
        vitD = vit;
        vitG = vit*((45.0f-alpha)/45.0f);
    }
    else
    {
        vitG = vit;
        vitD = vit*((alpha+45.0f)/45.0f);
    }
    if(nitroG)
    {
        if(vitG > 0)
            vitG = VITESSE_NITRO;
        else if(vitG < 0)
            vitG = -VITESSE_NITRO;
    }
    if(nitroD)
    {
        if(vitD > 0)
            vitD = VITESSE_NITRO;
        else if(vitD < 0)
            vitD = -VITESSE_NITRO;
    }

    vitG = 63*vitG/VITESSE_NITRO;
    vitD = 63*vitD/VITESSE_NITRO;

    sendPWMGauche((char)vitG);
    sendPWMDroit((char)vitD);
}

UART_Infos UARTInit() {
    UART_Infos Infos;
    Infos.Oo = initCom();
    Infos.hComm = link();

    if(Infos.hComm == NULL) printf("Error start XBee\n");

    return Infos;
}

void UARTClose(UART_Infos Infos) {
    if(Infos.hComm != NULL) CloseHandle(Infos.hComm);
}

void UARTPutChar(UART_Infos Infos, unsigned char c) {
    #ifdef UART_DEBUG
    printf("PC send        -> %2x   at%9d\n", c, SDL_GetTicks());
    #endif

    WriteABuffer((char*)(&c), 1, Infos.hComm, &(Infos.Oo));
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

HANDLE link() {
    DCB dcb;
    COMMTIMEOUTS timeouts;

    HANDLE hComm = CreateFile( "COM3",
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL);
    if (hComm == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    FillMemory(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(dcb);

    if (!BuildCommDCB("baud=115200 parity=N data=8 stop=1", &dcb)) {
        CloseHandle(hComm);
        return NULL;
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

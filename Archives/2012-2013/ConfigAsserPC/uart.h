#ifndef UART_H
#define UART_H

#include <windows.h>
#include <cstdio>

typedef struct {
    HANDLE hComm;
    OVERLAPPED Oo;
} UART_Infos;

UART_Infos UARTInit();
void UARTClose(UART_Infos Infos);
void UARTPutChar(UART_Infos Infos, unsigned char c);

class XBee
{
    public :
        XBee();
        ~XBee();
        void putChar(unsigned char c) const;
    private :
        UART_Infos uartInfos;
};


#endif

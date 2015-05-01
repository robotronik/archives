#ifndef UART_H
#define UART_H

#include <windows.h>
#include <cstdio>
#include "config.h"
#include <SDL/SDL.h>

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
        void putChar(unsigned char c);
        void sendPWMGauche(char value);
        void sendPWMDroit(char value);
        void sendVitAlpha(float vit, float alpha, bool nitroG, bool nitroD);
    private :
        UART_Infos uartInfos;
};


#endif

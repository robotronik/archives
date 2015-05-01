/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/


#ifndef _UART_H
#define _UART_H

#include <p33FJ128MC802.h>

// BRGVAL = 73,7 M / 32 / Baudrate - 1
//#define BRGVAL 23	// 9600 Bauds
#define BRGVAL 1	// 115200 Bauds

extern void sendSignalCalibrage(char aCalibrer);
extern volatile unsigned char semit;
extern volatile unsigned char toDoCalibrage;
extern volatile unsigned char debugToSend;

/* Functions headers */
void UART_init();
void UART_putc(unsigned char c);
void sendEmitN(unsigned char c);

void sendPosUpdated();
void sendCalibrageDone(); //00001111
void sendBeginCalibrage(unsigned char adr); // adr.10000

#endif

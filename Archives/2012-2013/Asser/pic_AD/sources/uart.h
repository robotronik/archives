/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



#ifndef _UART_H
#define _UART_H

/* Functions headers */

#define DONE 0x0D
#define ALPHA_GET 0xAA

int UART_putc(unsigned char c);

void sendDone(void);
void sendAlpha(long int alpha);

int send_long(long message);
int send_short(short message);

#endif

/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



#ifndef _UART_H
#define _UART_H

/* Functions headers */

int UART_putc(unsigned char c);

int send_long(long message);

#endif

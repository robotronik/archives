/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/

#ifndef _UART_H
#define _UART_H

#define INT_UART_TX

/* Functions headers */

void UART_putc(unsigned char c);
void UART_send_tab(unsigned char *addr, unsigned char size);

/* Retourne le nombre d'octets lus */
int UART_getc(unsigned char *byte);

#endif

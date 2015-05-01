/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



#ifndef UART_H
#define UART_H

/* Functions headers */

#define DONE 0x10
#define ALPHA_GET 0xAA
 
#define TX_BUFFER_SIZE 64

int UART_putc(unsigned char c);

void sendDone(void);
void sendDerapage(void);

#define sendAsser(v) send_short(v)
#define sendAsser32(v) send_long(v)

int send_long(long message);
int send_short(short message);

#endif /* UART_H */

/* 
 * File:   uart_xbee.h
 * Author: Dan
 *
 * Created on 17 avril 2013, 17:45
 */

#ifndef UART_XBEE_H
#define UART_XBEE_H

int send_xbee_long(long message) ;
int send_xbee_short(short message);
int UART_xbee_putc(unsigned char c);
void send_xbee_string();

#endif /* UART_XBEE_H */


#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <p33FJ128MC802.h>
#include "message.h"

struct RecepUS
{
	unsigned char 
		data:5,
		adr:3; //poids fort
};
typedef struct RecepUS RecepUS;

union Trame
{
	RecepUS recepUS;
	unsigned char c;
};
typedef union Trame Trame;


void UART_init();
void UART_putc(unsigned char c);
void send_message(long int message);

void sendEndCalibre();
void sendTn();
void sendPos();


#endif // UART_H_INCLUDED



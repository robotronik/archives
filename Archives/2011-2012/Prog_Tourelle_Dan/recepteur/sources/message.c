#include "message.h"
#include "uart.h"
#include <p33FJ128MC802.h>

volatile char n;
volatile char salve = 0;
volatile char calibre = 0;
volatile char tnAsked = 0;
volatile unsigned short tToSend = 0;
volatile char posAsked = 0;

void message_processing(unsigned char inMessage)//modifie toToDo et le copie dans toDo quand c'est complet
{
	Trame tr;
	tr.c = inMessage;
	if(tr.recepUS.adr != 0b111 && tr.recepUS.adr != ADRESSE)
		return;
	
	// Emit Trame
	if ( (tr.recepUS.data&0b00011100) == 0)
	{
		LATBbits.LATB5 = 0;//debug LED	allume
		n = tr.recepUS.data;
		salve =1;
		calibre = 0;
	}
	// GetPos
	else if(tr.recepUS.data == 0b11111)
	{
		posAsked = 1;
	}
	// Get Tn
	else if((tr.recepUS.data&0b00011100) == 0b00011000)
	{
		tnAsked = 1;
		tToSend = 0b0000000000000000/*toDo.time[0b11 & tr.c]*/;
	}
	// Calibre
	else if(tr.recepUS.data == 0b10000)
	{
		calibre = 1;
		salve = 0;
	}

	return;	
	
			
}

/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "comAsser.h"
#include "UART_Asser.h"
volatile int doneAsser=0;

#define TICK_TOUR 22538


extern volatile int posX;
extern volatile int posY;
extern volatile int alpha;

void sendAsser( unsigned short message)
{
	UART_Asser_PutChar((uint8) (message >> 8));
	UART_Asser_PutChar((uint8) message) ;
}

void sendDelta( short delta )
{
	TrameAsser t;
	if (delta < 0)
	{
		t.trDelta.delta = -delta;
		t.trDelta.signe = 1;
	}
	else
	{
		t.trDelta.delta = delta;
		t.trDelta.signe = 0;
	}
	t.trDelta.codet = CODE_DELTA;
	
	sendAsser(t.us);
}

void sendAlpha( short alpha )
{
	TrameAsser t;
	if (alpha < 0)
	{
		t.trAlpha.alpha = -alpha;
		t.trAlpha.signe = 1;
	}
	else
	{
		t.trAlpha.alpha = alpha;
		t.trAlpha.signe = 0;
	}
	t.trAlpha.codet = CODE_ALPHA;
	
	sendAsser(t.us);
}

void sendStop(void)
{
	TrameAsser t;
	t.trShort.Xo = CODE_STOP;
	
	sendAsser(t.us);
}

void getXY(void)
{
	TrameAsser t;
	t.trAlpha.codet = CODE_GET_XY;
	sendAsser(t.us);
}

void getAlpha(void)
{
	TrameAsser t;
	t.trAlpha.codet = CODE_GET_ALPHA;
	sendAsser(t.us);
}


void UART_Asser_RX_Handler(void)
{
	static int cpt = 0;
	static unsigned char messageType;
	unsigned char inMessage = UART_Asser_ReadRxData();
	//UART_XBee_PutChar(inMessage);//debug
	static TrameAsser t16;
	static Trame32 t32;
	
	if ( cpt == 0 )
	{
		t16.trShort.Xo = inMessage;
		t32.tr4Char.Xooo = inMessage;
		switch (t16.trAlpha.codet)
		{
			case (DONE_ASSER>>5) :
				//UART_XBee_PutString(" Fin Asser Vrai");
				doneAsser = 1;
				messageType = DONE_ASSER;
			break;
			case CODE_GET_ALPHA :
				cpt++;
				messageType = CODE_GET_ALPHA;
			break;
			case CODE_GET_XY :
				cpt++;
				messageType = CODE_GET_XY;
			break;
		}
	}
	else if ( cpt == 1)
	{
		t16.trShort.oX = inMessage;
		t32.tr4Char.oXoo = inMessage;
		switch(messageType)
		{
			case CODE_GET_ALPHA :
				cpt = 0;
				if(t16.trAlpha.signe ==1)
					alpha = -t16.trAlpha.alpha;
				else
					alpha = t16.trAlpha.alpha;
			break;
			case CODE_GET_XY :
				cpt++;
			break;
		}
	}
	else if(cpt == 2)
	{
		cpt++;
		t32.tr4Char.ooXo = inMessage;
	}
	else
	{
		cpt = 0;
		t32.tr4Char.oooX = inMessage;
		switch(messageType)
		{
			case CODE_GET_XY :
				posX = t32.tpos.x;
				posY = t32.tpos.y;
			break;
		}
	}
	
	
}

void waitDoneAsser(void)
{

	while(!doneAsser);
	doneAsser =0;
	
}

/* [] END OF FILE */

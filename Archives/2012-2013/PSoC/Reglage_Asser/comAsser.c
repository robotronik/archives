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

void sendAsser2( unsigned short message)
{
	UART_Asser_PutChar((uint8) (message >> 8));
	UART_Asser_PutChar((uint8) message) ;
}

void sendAsser4( unsigned long message)
{
	UART_Asser_PutChar((uint8) (message >> 24));
	UART_Asser_PutChar((uint8) (message >> 16));
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
	
	sendAsser2(t.us);
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
	
	sendAsser2(t.us);
}

void sendStop(void)
{
	TrameAsser t;
	t.trShort.Xo = 0x00;
	
	sendAsser2(t.us);
}

void sendConfig( int pid , int ad1 , int ad2, double val)
{
	TrameConfig tc;
	tc.conf.config = CODE_CONFIG;
	tc.conf.pid = pid;
	tc.conf.ad1 = ad1;
	tc.conf.ad2 = ad2;
	tc.conf.ent = (int)val;
	tc.conf.dec = (int)((val - (int)val)*100);
	
	sendAsser4(tc.l);

}

void UART_Asser_RX_Handler(void)
{
	unsigned char inMessage = UART_Asser_ReadRxData();
	
	
	
}

/* [] END OF FILE */

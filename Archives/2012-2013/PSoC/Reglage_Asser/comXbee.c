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
#include "UART_XBee_RX_isr.h"
#include "UART_XBee.h"
#include "comAsser.h"


void UART_XBee_RX_Handler(void)
{
	unsigned char inMessage = UART_XBee_ReadRxData();
	
	static int cpt = 0;
	static TrameConfig tc;
	UART_XBee_PutString("in");
	
	if ((inMessage >> 5) == CODE_CONFIG) //potentielement valide
	{
		cpt++;
		tc.tr4Char.Xooo = inMessage;
		UART_XBee_PutString("0");
	}
	else if (cpt == 1)
	{
		tc.tr4Char.oXoo = inMessage;
		cpt++;
		UART_XBee_PutString("1");
	}
	else if (cpt == 2)
	{
		tc.tr4Char.ooXo = inMessage;
		cpt++;
		UART_XBee_PutString("2");
	}
	else if (cpt == 3)
	{
		tc.tr4Char.oooX = inMessage;
		//configRecu = 1;
		sendConfig(tc.conf.pid, tc.conf.ad1, tc.conf.ad2, (double)(tc.conf.ent +  tc.conf.dec/100.));
		UART_XBee_PutString("Okrecu");
		cpt = 0;
	}
		
}

/* [] END OF FILE */

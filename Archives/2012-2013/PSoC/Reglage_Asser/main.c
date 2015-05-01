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
#include <device.h>

#include "UART_XBee.h"
#include "UART_Asser.h"
#include "comXbee.h"
#include "Depart.h"
#include "comAsser.h"

//volatile int configRecu = 0;

void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	UART_XBee_Start();
	UART_XBee_RX_isr_Start();
	UART_Asser_Start();
	UART_Asser_RX_isr_Start();
	isr_Fin_Start();
	
	//																																																																																																																	int val;
	
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
	
	CyDelay(1000);
	
	UART_XBee_PutString("Ready...");
	
	while (Depart_Read());
	Timer_Fin_Start();
	LED_Debug_Write(255);
	UART_XBee_PutString("Hello");
	
	
	/*sendDelta(1000);
	CyDelay(7000);
	UART_XBee_PutString(" FinDelay De ");
	
	sendDelta(300);
	CyDelay(5000);
	UART_XBee_PutString(" FinDelay De ");
	
	sendDelta(100);
	CyDelay(4000);
	UART_XBee_PutString(" FinDelay De ");
	
	sendDelta(2000);
	CyDelay(8000);
	UART_XBee_PutString(" FinDelay De ");*/
	
	sendAlpha(180);
	CyDelay(4000);
	UART_XBee_PutString(" FinDelay Al ");
	/*
	sendAlpha(180);
	CyDelay(4000);
	UART_XBee_PutString(" FinDelay Al ");*/
	/*
	sendAlpha(180);
	CyDelay(5000);
	UART_XBee_PutString(" FinDelay Al ");
	
	sendAlpha(110);
	CyDelay(4000);
	UART_XBee_PutString(" FinDelay Al ");
	
	sendAlpha(65);
	CyDelay(4000);
	UART_XBee_PutString(" FinDelay Al ");
	
	
	sendDelta(300);
	CyDelay(5000);
	UART_XBee_PutString(" FinDelay De ");
	
	sendDelta(-300);
	CyDelay(5000);
	UART_XBee_PutString(" FinDelay De ");
	*/
    for(;;)
    {
		//sendDelta(1000);
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

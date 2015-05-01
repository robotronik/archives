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
#include "device.h"

#include "UART_XBee.h"
#include "UART_Asser.h"
#include "comXbee.h"
#include "Depart.h"
#include "comAsser.h"
#include "machine.h"
#include "ballon.h"

volatile int posX;
volatile int posY;
volatile int alpha;
volatile int stopSharp;

void main()
{
    int i;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	UART_XBee_Start();
	UART_XBee_RX_isr_Start();
	UART_Asser_Start();
	UART_Asser_RX_isr_Start();
	isr_Fin_Start();
	
	ADCSharp_Start();
	
	//																																																																																																																	int val;
	Ballon_Write(0x00);
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
	
	CyDelay(1000);
	
	UART_XBee_PutString("Ready...");
	//CyDelay(3000);
	//Ballon_Write(0xFF);
	
	
	initTabDepl();
    for(;;)
    {
		machine();
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

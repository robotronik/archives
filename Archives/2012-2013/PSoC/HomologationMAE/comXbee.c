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


void UART_XBee_RX_Handler(void)
{
	unsigned char inMessage = UART_XBee_ReadRxData();
	
	
}

/* [] END OF FILE */

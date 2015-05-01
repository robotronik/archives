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

#include "device.h"

#define TOUR	(int)6773 		// [tck/tr/roue] = 500*distance entre roue(=264mm)/diamètre roue(=70mm)

void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	UART_XBee_Start();
	UART_XBee_RX_isr_Start();
	
	UART_Asser_Start();
	UART_Asser_RX_isr_Start();

	// Sharps

	
	//PWM_ServoGauche_Start();
	//PWM_ServoDroit_Start();
	

    CYGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */

	CyDelay(1000);

	// Init
	// Recuperation de la couleur

	
	Control_Led_Debug_Write(2); // Clignotement 1 Hz
	
	
	
	while(1)
	{
		
		
		
	}

	
}

/* [] END OF FILE */

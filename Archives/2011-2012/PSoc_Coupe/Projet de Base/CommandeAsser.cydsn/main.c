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
#include "machine.h"

#define Status_Depart_Read() 1  /// TODO : connecter la tirette

#define TOUR	(int)6773 		// [tck/tr/roue] = 500*distance entre roue(=264mm)/diamètre roue(=70mm)

void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	UART_XBee_Start();
	UART_XBee_RX_isr_Start();
	UART_Asser_Start();
	UART_Carto_Start();
	UART_Asser_RX_isr_Start();
	UART_Carto_RX_isr_Start();

    CYGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */

	CyDelay(1000);

	// Init
	initPssoc();
	
    //while(!Status_Depart_Read());
		//Control_Led_Debug_Write(2); // Clignotement 1 Hz
	Timer_85s_Start();
	isr_Fin_Start();
	
	signaux.timerOn = 1;

	for(;;)
	{
		runMachine();
	}
}

/* [] END OF FILE */

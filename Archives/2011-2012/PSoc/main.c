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
#include "message.h"

#define TOUR	(int)6773 		// [tck/tr/roue] = 500*distance entre roue(=264mm)/diamètre roue(=70mm)

void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	UART_XBee_Start();
	UART_Asser_Start();
	UART_Asser_RX_isr_Start();

    CYGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */
	
	CyDelay(1000);
	sendRED();	// Connaitre la couleur de notre robot (v 2011)
	
	// On recupere le chemin de la part de la carto
	
	// On l'envoie a l'asser
	
	sendXY_COURBE((long)0xFFFF*1400/3000, (long)0xFFFF*400/3000, (long)0xFFFF*5/3000);
	sendALPHA(0, 10);	
	
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

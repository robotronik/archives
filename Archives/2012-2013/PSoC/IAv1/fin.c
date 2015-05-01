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

#include "fin.h"
#include "UART_XBee.h"
#include "comAsser.h"
#include "Ballon.h"
#include "comXbee.h"
#include "constantes.h"

void fin(void)
{
	sendStop();
	sendRobot2Fin();
	CyDelay(1000);
	sendStop();
	sendRobot2Fin();
	CyDelay(3000);
	PRINTF("C'est la fin\r");
	Ballon_Write(0x00);
	PRINTF("Ballon open\r");
	CyDelay(3000);
	PRINTF("3s\r");
	CyDelay(1000);
	PRINTF("4s\r");
	CyDelay(3000);
	PRINTF("7s\r");
	Ballon_Write(0xFF);
	PRINTF("Ballon ferme\r");
	sendRobot2Fin();
	PRINTF("C'est vraiment la fin\r");
	while (1)
	{
		;	
	}
	PRINTF("### BUG ###\rApres while1\r");


}

/* [] END OF FILE */

/* ========================================
 *
 * Copyright Phelma, 2011
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF PHELMA.
 *
 * ========================================
*/

#include <device.h>

void main(void)
{
	init_capteur();
	
	CYGlobalIntEnable;	/* Enable all interrupts by the processor. */
	
    for(;;)
    {
    }
}

/* [] END OF FILE */

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

#include "debug.h"

void ledAfficheEntier(unsigned int n) {
	LED1_Write((n%2) ? 0 : 255);
	LED2_Write(((n>>1)%2) ? 0 : 255);
	LED3_Write(((n>>2)%2) ? 0 : 255);
	LED4_Write(((n>>3)%2) ? 0 : 255);
	LED5_Write(((n>>4)%2) ? 0 : 255);
	LED6_Write(((n>>5)%2) ? 0 : 255);
	LED7_Write(((n>>6)%2) ? 0 : 255);
}


void testStackFault(unsigned int testCode) {
	if(0x69 == testCode)
		LED1_Write(0);
	else
		LED1_Write(255);
}
/* [] END OF FILE */

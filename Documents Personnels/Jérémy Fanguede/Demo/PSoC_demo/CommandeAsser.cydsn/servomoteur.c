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

#include "main.h"

void setServoGauche(unsigned char ticks) {
	if (ticks > 180) ticks = 180;
	else if (ticks < 40 ) ticks = 40; // pour demo
	PWM_ServoGauche_WriteCompare(ticks + 60);
}

void setServoDroit(unsigned char ticks) {
	if (ticks > 180) ticks = 180;
	else if (ticks < 40 ) ticks = 40; ///pour demo
	PWM_ServoDroit_WriteCompare(240 - ticks + 10);	// + 10 pour symétrie
}

/* [] END OF FILE */

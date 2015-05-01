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

void setServoGauche(unsigned char angle) {
	if (angle > 180) angle = 180;
	PWM_ServoGauche_WriteCompare(angle + 60);
}

void setServoDroit(unsigned char angle) {
	if (angle > 180) angle = 180;
	PWM_ServoDroit_WriteCompare(240 - angle + 10);	// + 10 pour symétrie
}

/* [] END OF FILE */

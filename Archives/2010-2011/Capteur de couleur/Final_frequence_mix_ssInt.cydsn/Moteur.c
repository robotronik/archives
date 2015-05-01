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

void init_moteur(void) {
	PWM_1_Start();
	PWM_1_WriteCompare1(0);			// Arrête les moteurs
	PWM_1_WriteCompare2(0);			//		   ||
	
	isr_On_Off_Start();
}

void commande_moteurs(T_sensors sensors) {
	static uint8 speed_L, speed_R;
	
	switch (sensors.group) {
	case 1:								// Tourne a droite
		speed_L = V_RAPIDE;
		speed_R = V_LENT;
		break;
	case 2:								// Tourne a gauche
		speed_L = V_LENT;
		speed_R = V_RAPIDE;
		break;
	case 3:								// Avance droit
		speed_L = V_LENT;
		speed_R = V_LENT;
		break;
	/*default:							// Tourne sec
		if (speed_L > speed_R) {
			speed_L = V_LENT;
			speed_R = 0;
		} else {
			speed_L = 0;
			speed_R = V_LENT;
		}*/
	}
	PWM_1_WriteCompare1(speed_L*MOTOR_COEFF_L/100);
	PWM_1_WriteCompare2(speed_R);
}

/* [] END OF FILE */

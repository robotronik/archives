/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/
 
#ifndef _MOTORS_H
#define _MOTORS_H

#define MAX_SPEED 3999	// Entrée max des PWMs = 2*Période des PWMs
						// Fréquence des PWMs = 20 kHz
#define ASSER_PERIOD 5	// [demi-dixième de ms] postscaler des interruptions du PWM1
						// 5 -> 4 kHz

/* motors_set_speed : règle la vitesse d'un moteur, ie le rapport cyclique du PWM associé */
void motors_set_speed (short speedL, short speedR);
void motors_stop(void); // Arrete les moteurs

/* motors_get_qei : renvoie la lecture actuelle d'un codeur incrémentiel */
void motors_get_qei(long *pPosL, long *pPosR);

/* motors_reset_qei : réinitialise le codeur incrémentiel */
void motors_reset_qei();

#endif


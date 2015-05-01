/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/
 
#ifndef _MOTORS_H
#define _MOTORS_H

#define MAX_SPEED 3999//3999	// Entrée max des PWMs = 2*Période des PWMs
						// Fréquence des PWMs = 20 kHz
#define MIN_SPEED 800//520//1000//650	// Entrée min sauf pour l'arrêt
#define ASSER_PERIOD 5	// [demi-dixième de ms] postscaler des interruptions du PWM1
						// 5 -> 4 kHz (1 tck max par codeur par période d'asser)

//#define ACCEL_MAX_ROUE_INT	(short)((long)200000 * D / N / ASSER_PERIOD / ASSER_PERIOD)
//#define ACCEL_MAX_ROUE		1//(ACCEL_MAX_ROUE_INT >> 9)	// [periode²/512ème de tck] Temps entre chaque accélération de 1 = 1/(0,1 m/s²) (= 7)
#define ACCEL_MAX_ROUE2		1

/* motors_set_speed : règle la vitesse d'un moteur, ie le rapport cyclique du PWM associé */
void motors_set_speed (short speedL, short speedR);
void motors_stop(void); // Arrete les moteurs

/* motors_get_qei : renvoie la lecture actuelle d'un codeur incrémentiel */
void motors_get_qei(long *pPosL, long *pPosR);

/* motors_reset_qei : réinitialise le codeur incrémental */
void motors_reset_qei();


#endif

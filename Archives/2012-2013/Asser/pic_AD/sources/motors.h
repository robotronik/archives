/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/
 
#ifndef _MOTORS_H
#define _MOTORS_H

#define MAX_SPEED 3999//3999	// Entr�e max des PWMs = 2*P�riode des PWMs
						// Fr�quence des PWMs = 20 kHz
#define MIN_SPEED 800//520//1000//650	// Entr�e min sauf pour l'arr�t
#define ASSER_PERIOD 5	// [demi-dixi�me de ms] postscaler des interruptions du PWM1
						// 5 -> 4 kHz (1 tck max par codeur par p�riode d'asser)

//#define ACCEL_MAX_ROUE_INT	(short)((long)200000 * D / N / ASSER_PERIOD / ASSER_PERIOD)
//#define ACCEL_MAX_ROUE		1//(ACCEL_MAX_ROUE_INT >> 9)	// [periode�/512�me de tck] Temps entre chaque acc�l�ration de 1 = 1/(0,1 m/s�) (= 7)
#define ACCEL_MAX_ROUE2		1

/* motors_set_speed : r�gle la vitesse d'un moteur, ie le rapport cyclique du PWM associ� */
void motors_set_speed (short speedL, short speedR);
void motors_stop(void); // Arrete les moteurs

/* motors_get_qei : renvoie la lecture actuelle d'un codeur incr�mentiel */
void motors_get_qei(long *pPosL, long *pPosR);

/* motors_reset_qei : r�initialise le codeur incr�mental */
void motors_reset_qei();


#endif

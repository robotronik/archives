/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/
 
#ifndef _MOTORS_H
#define _MOTORS_H

#define MAX_SPEED 3999//3999	// Entr�e max des PWMs = 2*P�riode des PWMs
						// Fr�quence des PWMs = 20 kHz
#define MIN_SPEED 900	// Entr�e min sauf pour l'arr�t




/* motors_set_speed : r�gle la vitesse d'un moteur, ie le rapport cyclique du PWM associ� */
void motors_set_speed (short speedL, short speedR);
void motors_stop(void); // Arrete les moteurs

/* motors_get_qei : renvoie la lecture actuelle d'un codeur incr�mentiel */
void motors_get_qei(long *pPosL, long *pPosR);

/* motors_reset_qei : r�initialise le codeur incr�mental */
void motors_reset_qei();


#endif

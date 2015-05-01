/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/







#ifndef _MOTORS_H
#define _MOTORS_H

#define MAX_SPEED 4000 		// Entrée max des PWMs = 2*Période des PWMs : pas de sifflement

// externe

/* motors_set_speed : règle la vitesse d'un moteur, ie le rapport cyclique du PWM associé */
void motors_set_speed(Side motor, short speed);
void motors_stop(void); // Arrete les moteurs

/* motors_get_qei : renvoie la lecture actuelle d'un codeur incrémentiel*/
short motors_get_qei(Side motor);

/* motors_reset_qei : réinitialise le codeur incrémentiel */
void motors_reset_qei(Side motor);





#endif


/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/







#ifndef _MOTORS_H
#define _MOTORS_H



// externe

/* motors_set_speed : r�gle la vitesse d'un moteur, ie le rapport cyclique du PWM associ� */
void motors_set_speed(Side motor, int speed); // |speed| <= MAX_SPEED
void motors_stop(void); // Arrete les moteurs

/* motors_get_qei : renvoie la lecture actuelle d'un codeur incr�mentiel*/
int motors_get_qei(Side motor);

/* motors_reset_qei : r�initialise le codeur incr�mentiel */
void motors_reset_qei(Side motor);





#endif


/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/

#include "main.h"


/*
	Gauche : 1
	Droite : 2

	speed à mettre entre -400 et +400, 0 arrêt, +400 Vmax sens + et -400 Vmax sens -
*/
void motors_set_speed (Side motor, int speed)
{
    // Reglage de la vitesse commandee
    if (motor == LEFT)
    {
    	if (speed == 0)    // On s arrete
        {
	        PWM1CON1bits.PEN1H = 0;	// /!\ Reset le timer
	        PWM1CON1bits.PEN1L = 0;
        }
        else if(speed >= MAX_SPEED-10){
            PWM1CON1bits.PEN1H = 0;
            PWM1CON1bits.PEN1L = 1;
            P1DC1 = MAX_SPEED-10;
        }
        else if(speed <= -MAX_SPEED+10){
            PWM1CON1bits.PEN1H = 1;
            PWM1CON1bits.PEN1L = 0;
            P1DC1 = MAX_SPEED-10;
        }
		else if (speed > 0)  // On avance
        {
	        PWM1CON1bits.PEN1H = 0;
	        PWM1CON1bits.PEN1L = 1;
            P1DC1 = speed;
        }
    	else if (speed < 0)   // On recule
        {
	        PWM1CON1bits.PEN1H = 1;
	        PWM1CON1bits.PEN1L = 0;
            P1DC1 = -speed;
        }
    }

    if (motor == RIGHT)
    {
    	if (speed == 0)    // On s arrete
        {
	        PWM1CON1bits.PEN2H = 0;
	        PWM1CON1bits.PEN2L = 0;
        }
        else if(speed >= MAX_SPEED-10){
            PWM1CON1bits.PEN2H = 0;
            PWM1CON1bits.PEN2L = 1;
            P1DC2 = MAX_SPEED-10;
        }
        else if(speed <= -MAX_SPEED+10){
            PWM1CON1bits.PEN2H = 1;
            PWM1CON1bits.PEN2L = 0;
            P1DC2 = MAX_SPEED-10;
        }
		else if (speed > 0)  // On avance
        {
	        PWM1CON1bits.PEN2H = 0;
	        PWM1CON1bits.PEN2L = 1;
            P1DC2 = speed;
        }
    	else if (speed < 0)   // On recule
        {
	        PWM1CON1bits.PEN2H = 1;
	        PWM1CON1bits.PEN2L = 0;
            P1DC2 = -speed;
        }
    }
}

void motors_stop(void)
{
	// On met les 4 sorties en logique et on met tout a 0
	PWM1CON1bits.PEN1H = 0;
    PWM1CON1bits.PEN2H = 0;
    PWM1CON1bits.PEN1L = 0;
    PWM1CON1bits.PEN2L = 0;
}

int motors_get_qei(Side motor)
{
  return (motor == LEFT) ? POS1CNT : POS2CNT;
}

void motors_reset_qei(Side motor)
{
    if (motor == LEFT)
        POS1CNT = 0;
    else
        POS2CNT = 0;
}


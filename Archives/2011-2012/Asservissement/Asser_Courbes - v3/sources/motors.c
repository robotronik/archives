/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/

#include "main.h"

extern T_dividedLong distL, distR;

/*
	Gauche : 1
	Droite : 2
*/
void motors_set_speed (short speedL, short speedR)
{
   	if (speedL == 0)    // On s arrete
    {
        PWM1CON1bits.PEN1H = 0;	// /!\ Reset le timer
        PWM1CON1bits.PEN1L = 0;
    }
    else if(speedL >= MAX_SPEED){
        PWM1CON1bits.PEN1H = 0;
        PWM1CON1bits.PEN1L = 1;
        P1DC1 = MAX_SPEED-10;
    }
    else if(speedL <= -MAX_SPEED){
        PWM1CON1bits.PEN1H = 1;
        PWM1CON1bits.PEN1L = 0;
        P1DC1 = MAX_SPEED;
    }
	else if (speedL > 0)  // On avance
    {
	    PWM1CON1bits.PEN1H = 0;
	    PWM1CON1bits.PEN1L = 1;
	    P1DC1 = speedL;
    }
   	else if (speedL < 0)   // On recule
    {
        PWM1CON1bits.PEN1H = 1;
        PWM1CON1bits.PEN1L = 0;
        P1DC1 = -speedL;
    }
    
   	if (speedR == 0)    // On s arrete
    {
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 0;
    }
    else if(speedR >= MAX_SPEED){
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 1;
        P1DC2 = MAX_SPEED;
    }
    else if(speedR <= -MAX_SPEED){
        PWM1CON1bits.PEN2H = 1;
        PWM1CON1bits.PEN2L = 0;
        P1DC2 = MAX_SPEED;
    }
	else if (speedR > 0)  // On avance
    {
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 1;
        P1DC2 = speedR;
    }
   	else if (speedR < 0)   // On recule
    {
        PWM1CON1bits.PEN2H = 1;
        PWM1CON1bits.PEN2L = 0;
        P1DC2 = -speedR;
    }
}

void motors_stop(void)
{
	// On met les 4 sorties en logique
	PWM1CON1bits.PEN1H = 0;
    PWM1CON1bits.PEN2H = 0;
    PWM1CON1bits.PEN1L = 0;
    PWM1CON1bits.PEN2L = 0;
}

short motors_get_qei(Side motor)
{
  return (motor == LEFT) ? POS1CNT : POS2CNT;
}

void motors_reset_qei(Side motor)
{
    if (motor == LEFT) {
        POS1CNT = 0;
        distL.part.high = 0;
    } else {
        POS2CNT = 0;
        distR.part.high = 0;
    }    
}

void __attribute__((interrupt, auto_psv)) _QEI1Interrupt(void)
{
	IFS3bits.QEI1IF = 0; // On s'acquitte de l'interruption
	if ((unsigned short) POS1CNT < 32768) distL.part.high ++;
	else distL.part.high --;
}

void __attribute__((interrupt, auto_psv)) _QEI2Interrupt(void)
{
	IFS4bits.QEI2IF = 0; // On s'acquitte de l'interruption
	if ((unsigned short) POS2CNT < 32768) distR.part.high ++;
	else distR.part.high --;
}


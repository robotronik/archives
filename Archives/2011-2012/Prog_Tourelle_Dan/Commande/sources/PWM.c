#include "PWM.h"

void init_pwm()
{
    P1TPER = PERIODE_PWM;		// Période du timer
    P1DC1 =  PERIODE_PWM;      	// Duty-cycle PWM1H1 = 50% ?
    P1DC2 =  PERIODE_PWM;      	// Duty-cycle PWM1H2 = 50% ?

    PWM1CON1bits.PMOD1 = 1;     // Sorties PWM1H1 et PWM1L1 indépendantes
    PWM1CON1bits.PMOD2 = 1;     // Sorties PWM1H2 et PWM1L2 indépendantes

	OC1CONbits.OCTSEL = 1;		// Timer 3
    P1TCONbits.PTEN = 1;        // Active le Timer des PWMs
    P1TCONbits.PTOPS = 0;	    // Pas de postscaler
	IEC3bits.PWM1IE = 0;		// Interdiction de l'interruption

	_TRISB14=1; // Juste pour empecher d ecrire sur Pin 26
}

void emitUS_0()
{
	sendEmitN(0);
	PWM1CON1bits.PEN2H = 1;
	PWM1CON1bits.PEN2L = 0;
	PWM1CON1bits.PEN1H = 0;
}

void emitUS_1()
{
	sendEmitN(1);
	PWM1CON1bits.PEN2H = 0;
	PWM1CON1bits.PEN2L = 1;
	PWM1CON1bits.PEN1H = 0;
}

void emitUS_2()
{
	sendEmitN(2);
	PWM1CON1bits.PEN2H = 0;
	PWM1CON1bits.PEN2L = 0;
	PWM1CON1bits.PEN1H = 1;
}

void disableEmit()
{
	PWM1CON1bits.PEN2H = 0;
	PWM1CON1bits.PEN2L = 0;
	PWM1CON1bits.PEN1H = 0;
}

void emitCalibrage()
{
	PWM1CON1bits.PEN2H = 1;
	PWM1CON1bits.PEN2L = 1;
	PWM1CON1bits.PEN1H = 1;
}

#include "motors.h"
#include <p33Fxxxx.h>
#include "constants.h"

#define protect(v) (v > MAX_SPEED ? MAX_SPEED : v)

void initPWM(void)
{
    P1TCONbits.PTEN = 1; //PWM Time Base Timer Enable bit
    P1TCONbits.PTOPS = 0b0010; //4 PWM Time Base Output Postscale Select bits
    P2TCON = P1TCON;

    P1TPER = MAX_SPEED/2;
    P2TPER = MAX_SPEED/2;

    PWM1CON1bits.PMOD1 = 1;
    PWM1CON1bits.PMOD2 = 1;
    PWM1CON1bits.PMOD3 = 1;
    PWM2CON1bits.PMOD1 = 1;

    PWM1CON1bits.PEN1H = 0;
    PWM1CON1bits.PEN2H = 0;
    PWM1CON1bits.PEN3H = 0;
    PWM2CON1bits.PEN1H = 0;
    PWM1CON1bits.PEN1L = 1;
    PWM1CON1bits.PEN2L = 1;
    PWM1CON1bits.PEN3L = 1;
    PWM2CON1bits.PEN1L = 1;

    P1DC1 = 0;
    P1DC2 = 0;
    P1DC3 = 0;
    P2DC1 = 0;

}



void motorsSetSpeedM1(unsigned short m)
{
    P1DC1 = U(protect(m)); //TODO U() inutile unsigned !!
}

void motorsSetSpeedM2(unsigned short m)
{
    P1DC2 = U(protect(m));
}

void motorsSetSpeedM3(unsigned short m)
{
    P1DC3 = U(protect(m));
}

void motorsSetSpeedM4(unsigned short m)
{
    P2DC1 = U(protect(m));
}

void motorsSetSpeed(unsigned short m1, unsigned short m2, unsigned short m3, unsigned short m4)
{
    P1DC1 = U(protect(m1));
    P1DC2 = U(protect(m2));
    P1DC3 = U(protect(m3));
    P2DC1 = U(protect(m4));
}

void motorsStop(void)
{
    motorsSetSpeed(0,0,0,0);
}



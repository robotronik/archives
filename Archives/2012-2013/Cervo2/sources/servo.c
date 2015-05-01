#include "servo.h"
#include "main.h"

extern volatile Pos robotNous;
extern Pos objectif;

void initPWM(void)
{
    P1TCONbits.PTEN = 1; //PWM Time Base Timer Enable bit
    P1TCONbits.PTOPS = 0b0010; //4 PWM Time Base Output Postscale Select bits
    P1TCONbits.PTCKPS = 0b11;
    P1TPER = 2000;

    PWM1CON1bits.PMOD1 = 1;
    PWM1CON1bits.PMOD2 = 1;
    PWM1CON1bits.PEN1H = 1;
    PWM1CON1bits.PEN1L = 1;
    PWM1CON1bits.PEN2H = 1;
    PWM1CON1bits.PEN2L = 1;

    P1DC1 = 0;
    P1DC2 = 0;

}

void monterBras(void)
{
    if(objectif.alpha==0)
    {
        P1DC2=1350;
    }
    else
    {
        P1DC1=2200;
    }
    pause_ms(1000);

}

void baisserBras(void)
{
    P1DC1=750;
    P1DC2=2800;//DC1 = 750
    pause_ms(1000);
}

void lacherBras(void)
{
    P1DC2=0;
    P1DC1=0;
}

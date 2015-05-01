#include "servo.h"
#include "main.h"
#include "debug.h"

extern volatile Pos robotNous;
extern Pos objectif;

void initPWM(void)
{
    TRIS_SERVO_1 = 0;
    TRIS_SERVO_2 = 0;
    TRIS_SERVO_3 = 0;
    TRIS_SERVO_4 = 0;

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

    SERVO = 0;
    SERVO = 0;
}

void baisser_Porte(void)
{
    SERVO=2200;
    pause_ms(330);
}

void lever_Porte(void)
{
    SERVO=750;
    pause_ms(1000);
}

void lacher_Porte(void)
{
    //P1DC2=0;
    SERVO=0;
}

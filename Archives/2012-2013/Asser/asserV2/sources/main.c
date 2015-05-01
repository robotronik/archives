/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "motors.h"
#include "callage.h"
#include "init.h"

//#include "bitsConfiguration.h"
volatile int isPSoCReady = FALSE;

extern long int AD_consAlpha; //TICK_TOUR;//-10*TICK_TOUR;//0;
extern long int AD_consDelta;
extern long int AD_minSpeed;
long int tickG = 0, tickD = 0;

int main() {

    long int vG=-5000, vD=-5000;
    init();

    //AD_motors_set_speed(3999,3999);
    /*
    while (!isPSoCReady) {
            UART_putc(TEST);
            pause_ms(10);
    }*/
    //motors_set_speed (SPEED,SPEED);
    //while(1);

int k;
    /*while (tickG > -5 &&tickD > -5)
    {
        vG -= 20;
        vD -= 20;
        tickG = 0;
        tickD = 0;
        motors_reset_qei();
        motors_set_speed(echelle(vG), echelle(vD));
        pause_ms(70);
        motors_get_qei(&tickG, &tickD);
        for(k = 0; k<10; k++){}
        pause_ms(100);
    }
    motors_set_speed(0, 0);*/
    AD_minSpeed = 7000; (long int)((float)vG*(-2));
    //while(1);
    pause_s(1);
    asser();
    //callage();
    return 0;
}



/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "motors.h"
#include "init.h"

//#include "bitsConfiguration.h"
volatile int isPSoCReady = FALSE;





int main() {
        init();

       //AD_motors_set_speed(echelle(AD_MIN_SPEED),echelle(AD_MIN_SPEED));
	/*
	while (!isPSoCReady) {
		UART_putc(TEST);
		pause_ms(10);
	}*/
	//motors_set_speed (SPEED,SPEED);
	//while(1);




	asser();
	//callage();
	return 0;
}



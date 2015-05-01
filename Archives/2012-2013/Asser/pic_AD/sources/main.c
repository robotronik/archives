/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "motors.h"
#include "callage.h"
volatile int isPSoCReady = FALSE;

//#define SPEED 600

#ifndef SIMU
int main() {
#else
int main(int argc, char** argv) {
#endif

	init();
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

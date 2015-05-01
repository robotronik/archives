/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "motors.h"

volatile int isPSoCReady = FALSE;

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
	asser();
	
	return 0;
}

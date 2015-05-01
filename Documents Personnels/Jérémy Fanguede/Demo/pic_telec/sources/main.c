zdzzss/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

// Les divisions par des constantes "long" ne fonctionnent pas ?

#include "main.h"
#include "message.h"
#include "motors.h"

#define SPEED 2000

volatile int isPSoCReady = FALSE;

unsigned char toDo = 's'; 
unsigned char recu = 0;

#ifndef SIMU
int main() {
#else
int main(int argc, char** argv) {
#endif

	init();
	while (!isPSoCReady)
		 {
		UART_putc(TEST);
		pause_ms(10);
	}
	while (1)
	{
		if (recu)
		{
			if (toDo == 's')
			{
				 motors_set_speed(0, 0);
			}
			else if (toDo == 'a')
			{
				motors_set_speed(SPEED, SPEED);
			}	
			else if (toDo == 'r')
			{
				motors_set_speed(-SPEED, -SPEED);
			}	
			else if (toDo == 'g')
			{
				motors_set_speed(-SPEED, SPEED);
			}	
			else if (toDo == 'd')
			{
				motors_set_speed(SPEED, -SPEED);
			}
			recu = 0;	
		}
	}	
			 
	asser();
	
	return 0;
}

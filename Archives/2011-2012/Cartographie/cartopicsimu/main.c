#include "help_main.h"

int main()
{
	init_main();

	/*
	while(1)
	{
		UART_putc_carto(0xAA);
		unsigned int i;
		for(i=0; i<10000; i++);
	}
*/

	while (1)
	{
		boucle();
	}
	return 0;
}


/*-----------------------------------------------------*
 * Utile pour du debug                                 *
 *-----------------------------------------------------*/

#include "main.h"

void allumer_del(void)
{
	_RB5 = 0; // nouvelle carte
}

void eteindre_del(void)
{
	_RB5 = 1; // nouvelle carte
}

void pause_ms(unsigned int n)
{
	int i,j;
	for(j=0; j<n; j++)
	{
     	for(i=0; i<400; i++)
			Nop();
	}
}

void pause_s(unsigned int n)
{
	int i;
	for(i=0; i<n; i++)
		pause_ms(1000);
}

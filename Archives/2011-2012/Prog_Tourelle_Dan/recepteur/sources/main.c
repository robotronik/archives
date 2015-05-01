#include <p33FJ128MC802.h>
#include "calibreur.h"

#define NB_ECHECS_MAX 5

#include "init.h"
#include "ADC.h"
#include "timer.h"
#include "uart.h"
#include "message.h"

unsigned short x,y; // Nos positions
unsigned char nbEchecs; // Nombre d'echecs consecutifs

void updateTime()
{
	// On regarde si la valeur est chargeable
	unsigned short tRecep = getDtus();
	if(tRecep <= TIME_OUT)		// On ne prend pas de valeurs incoherentes pour t (t trop grand)
	{
		toDo.time[toDo.nbUsRecues] = tRecep;
	}
	else
	{
		toDo.echec = 1;
	}
	toDo.nbUsRecues ++;
}

void updateXY()
{
	// On traite les erreurs
	if(toDo.echec)
	{
		if(nbEchecs >= NB_ECHECS_MAX)
		{
			x = 0xFFFF;
			y = 0xFFFF;
			return;
		}
		else
			nbEchecs++;
	}
	else
		nbEchecs = 0;

	// On fait l'update
	unsigned int sommeT = (unsigned int)toDo.time[0] + (unsigned int) toDo.time[1] + (unsigned int) toDo.time[2];
	
	x = (((unsigned int)toDo.time[1])*300)/sommeT;
	y = (((unsigned int)toDo.time[0])*200 + ((unsigned int)toDo.time[1])*100)/sommeT;
}

// Programme de test de reception / echantillonage
void testRecepSeuil()
{
	_RB5 = 1;
	short U = 0;
	while(1)
	{
		U = getADCValue();
		if(U > seuil)
			_RB5 = 0;
		else
			_RB5 = 1;
	}
}

int main()
{
	// On init le materiel
	init_osc();
	initADC();
	UART_init();
	initTimer();
	_TRISB5 = 0;

	// On init nos variables
	x = 0xFFFF;
	y = 0xFFFF;
	nbEchecs = 0;

	//testRecepSeuil();

	n =0;
	salve =0;
<<<<<<< .mine
=======

	/*
	while(1)
	{
		UART_putc(0xAA);
	}
	/*/

>>>>>>> .r229
	
	while (1)
	{

	   	
		if(calibre == 1)
		{
			calibrer();
			calibre = 0;
		}
		else if (salve == 1)
		{
			//debut timer;
			startTimer();
			while ( getADCValue() < seuil  || timeOut());
			// On a recu la trame ou bien le temps est ecoule
			stopTimer();
			updateTime();
			if (toDo.nbUsRecues == 3)
			{
				updateXY(); // On charge les nouvelles valeurs
				
				initToDo();
				LATBbits.LATB5 = 1;//debug LED eteint
				
				
			}	
			salve =0;
		}
		else if(tnAsked)
		{
			sendTn();
			tnAsked = 0;
		}
		else if(posAsked)
		{
			sendPos();
			posAsked = 0;
		}
	
	
	}
	return 0;
}



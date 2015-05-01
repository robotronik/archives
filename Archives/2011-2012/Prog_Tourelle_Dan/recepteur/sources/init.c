#include "init.h"

ACalculer toDo;	

void init_osc()
{
/*	// Init de la PLL
	#ifndef SIMU
	CLKDIVbits.PLLPRE = 0;		// (FRC) /2
	PLLFBD = 41;				// (FRC/2) *43
	CLKDIVbits.PLLPOST = 0;		// (FRC/2*43) /2

	__builtin_write_OSCCONH((OSCCONH | 1 )& 0xF9);	// Choix de l'horloge FRC avec PLL
	__builtin_write_OSCCONL(OSCCONL | 1);			// Changement d'horloge
	//while (!OSCCONbits.LOCK);						// Attend le bloquage de la PLL (debug)
	while (OSCCONbits.OSWEN); 						// Attend le changement
	#endif*/
}

void initToDo()
{
	toDo.nbUsRecues = 0;
	toDo.echec = 0;
	toDo.time[0] = -1;
	toDo.time[1] = -1;
	toDo.time[2] = -1;
	return;
}	

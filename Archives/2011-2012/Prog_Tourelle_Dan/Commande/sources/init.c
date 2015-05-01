
#include "init.h"
void init()
{

    TRISBbits.TRISB5 = 0;//output led de test
    LATBbits.LATB5 =0; //allumé
    
    
    //Activatin du controle des tours 
    _TRISB14=0;
    _TRISB12=0;
    _TRISB13=0;

	
	
//	init_osc();
	initTimer();
	UART_init();
	
}


void init_osc()
{
	// Init de la PLL
	#ifndef SIMU
	CLKDIVbits.PLLPRE = 0;		// (FRC) /2
	PLLFBD = 41;				// (FRC/2) *43
	CLKDIVbits.PLLPOST = 0;		// (FRC/2*43) /2

	__builtin_write_OSCCONH((OSCCONH | 1 )& 0xF9);	// Choix de l'horloge FRC avec PLL
	__builtin_write_OSCCONL(OSCCONL | 1);			// Changement d'horloge
	//while (!OSCCONbits.LOCK);						// Attend le bloquage de la PLL (debug)
	while (OSCCONbits.OSWEN); 						// Attend le changement
	#endif
}




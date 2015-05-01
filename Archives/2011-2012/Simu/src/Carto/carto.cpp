#include "carto.h"

#ifndef SIMU
const unsigned char heap[HEAP_SIZE];
#endif
#ifdef SIMU
unsigned char heap[HEAP_SIZE];
#endif
unsigned char* heap_ptr;
unsigned char heap2[HEAP_SIZE2];
unsigned char* heap_ptr2;


void init()
{
    #ifndef SIMU
    TRISBbits.TRISB5 = 0;//output
    LATBbits.LATB5 = 1;
	/*TRISBbits.TRISB3 = 0;//output
    LATBbits.LATB3 = 1;//valeur de la sortie
	TRISBbits.TRISB2 = 0;//output
    LATBbits.LATB2 = 0;//valeur de la sortie*/
    #endif
	init_osc();
	UART_init();
	my_malloc_init();

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

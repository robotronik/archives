/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/





#include "main.h"

extern long consigneDelta, consigneAlpha;
extern int rapportVitesse;
extern char doitAttendre;
extern long erreurDelta, erreurAlpha;

extern int Vg;
extern int Vd;


int main()
{
	init();

	pause_s(1);
	motors_stop();
	
	/* Tests */
	/*
	//consigneDelta = (long)720*RAPPORT_CONVERSION;	// 1 tour autour d'une roue
	consigneDelta = 216658;								// 1 tour autour d'une roue
	rapportVitesse = (((long)720*RAPPORT_CONVERSION) << 16)/consigneDelta;	// de 0 (droite) à 1
																			// >> 16 à l'utilisation
																			// Si > 1 : asser sur alpha//*/
	//send_message(rapportVitesse);
	/*
	consigneAlpha = 0;
	consigneDelta = (long)14000*N/D;	// 2 cases
	rapportVitesse = 0;//*/
	//*
	consigneDelta = 0;
	consigneAlpha = (long)720*RAPPORT_CONVERSION;	// 1 tour
	rapportVitesse = 3949;//*/	(consigneDelta = 13061)
	
	erreurAlpha += consigneAlpha;
	erreurDelta += consigneDelta;

	while(1) {



 		motors_set_speed(LEFT, Vg);
		motors_set_speed(RIGHT, Vd);


		// if (consigneDelta)
		// 	while(1)
		//  	{
		// 		while (doitAttendre);			// Attend l'interruption du Timer
		// 		doitAttendre = TRUE;
		// 		asser_delta();
		// 		if (!doitAttendre) _RB5 = 0;	// Timer trop rapide : allume LED debug
		// 	}
		// if (consigneAlpha)
		// 	while(1)
		//  	{
		// 		while (doitAttendre);			// Attend l'interruption du Timer
		// 		doitAttendre = TRUE;
		// 		asser_alpha();
		// 		if (!doitAttendre) _RB5 = 0;	// Timer trop rapide : allume LED debug
		// 	}
	}		

    return 0;
}

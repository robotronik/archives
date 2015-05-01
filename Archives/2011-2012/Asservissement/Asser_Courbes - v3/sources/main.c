/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

// Les divisions par des constantes "long" ne fonctionnent pas



#include "main.h"

extern long consigneDelta, consigneAlpha;
extern int rapportVitesse;

int main()
{
	init();

	pause_s(1);

	while(1) {
		/** Tests **/
		int i;
		for (i=0; i<4; i++) {
			consigneDelta = conversion_delta_diziememm_vers_pt(4000);
			consigneAlpha = conversion_alpha_deg_vers_pt(0);
			
			//*
			rapportVitesse = ((long long)consigneAlpha << 15) / consigneDelta;	// de 0 (droite) à +-1
																				// >> 15 à l'utilisation
			consigneAlpha = 0;
			asser_delta(); 	// Boucle d'asser en delta //*/
			
			//pause_s(1);
			
			consigneDelta = conversion_delta_diziememm_vers_pt(1500);
			consigneAlpha = conversion_alpha_deg_vers_pt(90);
			
			//*
			rapportVitesse = ((long long)consigneDelta << 15) / consigneAlpha;	// de 0 (tour) à +-1
																				// >> 15 à l'utilisation
			consigneDelta = 0;
			asser_alpha(); 	// Boucle d'asser en alpha //*/
			
			//pause_s(1);
		}
		
		//*
		consigneDelta = conversion_delta_diziememm_vers_pt(0);
		consigneAlpha = conversion_alpha_deg_vers_pt(-360);
		
		/*
		rapportVitesse = ((long long)consigneAlpha << 15) / consigneDelta;	// de 0 (droite) à +-1
																			// >> 15 à l'utilisation
		consigneAlpha = 0;
		asser_delta(); 	// Boucle d'asser en delta //*/
		
		//*
		rapportVitesse = ((long long)consigneDelta << 15) / consigneAlpha;	// de 0 (tour) à +-1
																			// >> 15 à l'utilisation
		consigneDelta = 0;
		asser_alpha(); 	// Boucle d'asser en alpha //*/
		
		//pause_s(1);
		
		/*/ Clignotement de la LED de Débug
		while (1) {
			_RB5 = 0;
			pause_s(1);
			_RB5 = 1;
			pause_s(1);
		} //*/
	}		

    return 0;
}

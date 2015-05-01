/*-----------------------------------------------------*
 * Conversion grandeurs                                *
 *-----------------------------------------------------*/

#include "main.h"

long conversion_alpha_pt_vers_deg(long a){
	return a/RAPPORT_CONVERSION/2;
}

long conversion_alpha_deg_vers_pt(long a){    // tourne vers la droite avec une valeur positive
	return a*RAPPORT_CONVERSION*2;
}

long conversion_delta_pt_vers_diziememm(long a){
	return a*(long)D/N/2;
}

long conversion_delta_diziememm_vers_pt(long a){ // avance vers l'avant avec une valeur positive
	return 2*a*(long)N/D;
}

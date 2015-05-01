/*-----------------------------------------------------*
 * Conversion grandeurs                                *
 *-----------------------------------------------------*/




#include "main.h"



long conversion_alpha_pt_vers_mdeg(long a){
	return a*(long)RAPPORT_CONVERSION;
}

long conversion_alpha_mdeg_vers_pt(long a){    // tourne vers la droite avec une valeur positive
	return a*100/(long)RAPPORT_CONVERSION;
}

long conversion_alpha_inMessage_vers_pt(unsigned short inMessage){

	//test si la valeur est négative, conversion de celle-ci en mdeg selon le protocole de com et appel de la fonction de conversion
	if(inMessage & (1<<11)) 
		return conversion_alpha_mdeg_vers_pt(- (long)(inMessage & 0x7FF)*200);
	else
		return conversion_alpha_mdeg_vers_pt((long)inMessage*200);
}

unsigned short conversion_alpha_pt_vers_outMessage(long a){
	
	// appel de la conversion, test si la valeur est negative et conversion de celle-ci en 0.2deg selon le protocole de com
	a = conversion_alpha_pt_vers_mdeg(a);
	if(a<0){
		return (1<<11) | ((unsigned short)(-a) >>1); 
	}
	else
		return (unsigned short)(a) >>1;
}

long conversion_delta_pt_vers_diziememm(long a){
	return a*(long)D/N;
}

long conversion_delta_diziememm_vers_pt(long a){ // avance vers l'avant avec une valeur positive
	return a*(long)N/D;
}

long conversion_delta_inMessage_vers_pt(unsigned short inMessage){

	//test si la valeur est négative, conversion de celle-ci en dizieme de mm selon le protocole de com et appel de la fonction de conversion
	if(inMessage & (1<<11)) // la valeur est negative
		return conversion_delta_diziememm_vers_pt(- (long)(inMessage & 0x7FF)*5);
	else
		return conversion_delta_diziememm_vers_pt((long)inMessage*5);
}

unsigned short conversion_delta_pt_vers_outMessage(long a){

	// appel de la conversion, test si la valeur est negative et conversion de celle-ci en 0.5mm selon le protocole de com
	a = conversion_delta_pt_vers_diziememm(a);
	if(a<0){
		return (1<<11) | ((unsigned short)(-a/5)); 
	}
	else
		return (unsigned short)(a/5);
}




/*
long passage_da_dd_vers_dx(float a, float d){
	return (long)(d*cos(a));
	//x+= d_delta_cm*cos(d_alpha_rad)
}

long passage_da_dd_vers_dy(float a, float d){
	return (long)(d*sin(a));
	//y+= d_delta_cm*sin(d_alpha_rad)
}

long passage_dx_dy_vers_dd(float a, float d){
	return (long)(sqrt( a*a + d*d ));
//delta_cm=sqrt( Dx²+Dy² )
}

long passage_dx_dy_vers_da(float a, float d){
	return atan(d/a);
//alpha_cm=arctan ( Dy/Dx )
}
//*/


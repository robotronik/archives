/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/



#ifndef _ASSER_H
#define _ASSER_H

#include "main.h"

// 1 tck = distance parcourue par une roue codeuse pour que le codeur s'incrémente




//////////////////////////////////////////
#define PI 3.1415926535897932384626433832795
#define DEUX_PI 6.283185307179586476925286766559
#define TICK_TOUR 5841//Nombre de tick pour faire un tour
#define TICK_ROUE 1048 //Nombre de cran/tick par tour de roue

#define COEFF 8000 //12
#define AD_MAX_SPEED 39990//39990
#define AD_MAX_SPEED_REEL 35000
#define AD_MIN_SPEED  20000//8000//10000

#define VIT_MAX 10000//15000

#define TICKS_TO_MM 0.07957863// Non divisé par 2 : 0,215834609788611 // mm/tick;
#define MM_TO_TICKS 12.5661872//22.084 // tick/mm
//#define ENTRE_AXE_TICKS 2627.2211093803600792287934402566// 1021.0176124166828025003590995658 // en mm 325


#define SEUIL_DERAP 1.1


#define AD_abs(valeur) ((valeur) < 0 ? -(valeur) : (valeur) )
#define AD_max(a,b) ((AD_abs(a) > AD_abs(b)) ? AD_abs(a) : AD_abs(b))


long int deriv_erreurs(long int erreurs[4]);
long int rampe(long int vcommande, long int prec);
long int echelle(long int c);

void AD_motors_set_speed(short speedL, short speedR);
void majXYAngle(double dTheta, double dDelta);

long int moyenne_erreurs( long int erreurs[4]);
void maj_erreurs( long int erreurs[4],long int erreur);

void setGros();
void setPrecis();



struct AD_K 
{
	/*double delta;
	double alpha;*/
	long int delta;
	long int alpha;
};
typedef struct AD_K AD_K; 	

//////////////////////////////////////////

void asser();

#endif

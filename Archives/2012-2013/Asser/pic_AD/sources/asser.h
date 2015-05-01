/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/



#ifndef _ASSER_H
#define _ASSER_H

#include "main.h"

// 1 tck = distance parcourue par une roue codeuse pour que le codeur s'incrémente



#define NB_SIN		(short)11269            // [tck/quart de tr] = (TOUR+1)/2	GÉNÉRÉ PAR GeneSin.exe : NE PAS MODIFIER, LAISSER DES ESPACES APRES





long int moyenne_erreurs( long int erreurs[4]);
void maj_erreurs( long int erreurs[4],long int erreur);

//////////////////////////////////////////
#define TICK_TOUR 16330.//16507.31707//22538. //11414.6341 //Nombre de tick pour faire un tour 
#define TICK_ROUE 1440 //Nombre de cran/tick par tour de roue

#define ACCELERATION_MAX 2
#define COEFF 12

#define VIT_MAX 2000

#define TICKS_TO_MM 0.09056330 //0.08944812416470939081733915188504 // 41Pi/1440 en mm/ticks
#define MM_TO_TICKS 11.042//11.179664295235574805228908256411  // (1440/41Pi) en ticks/mm
//#define ENTRE_AXE_TICKS 2627.2211093803600792287934402566// 1021.0176124166828025003590995658 // en mm 325

#define AD_abs(valeur) (valeur < 0 ? -valeur : valeur )
#define AD_max(a,b) ((AD_abs(a) > AD_abs(b)) ? AD_abs(a) : AD_abs(b))

void majAlpha(void);
long int deriv_erreurs(long int erreurs[4]);
long int rampe(long int vcommande, long int prec);
void AD_motors_set_speed(short speedL, short speedR);
void majXYAngle(long int alpha, long int newDelta);
			



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

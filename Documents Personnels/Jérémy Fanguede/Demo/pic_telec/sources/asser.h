/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/



#ifndef _ASSER_H
#define _ASSER_H

#include "main.h"

// 1 tck = distance parcourue par une roue codeuse pour que le codeur s'incrémente

#define METRE	(short)22062				// [demi-tck] Informatif; = 2*1440[tcks]*1000[mm]/PI/diamètre roue(=41mm)
	//!\ TOUR : PSoC à modifier
#define TOUR	(long)22538 				// [tck/tr] = 2*1440[tcks]*LARGEUR/diamètre roue(=41mm)
#define COEFF_ALPH_DELT	3587				// [demi-tck / rad]=[demi-tck] = TOUR/(2*PI)
#define NB_SIN	(short)11269                // [tck/quart de tr] = (TOUR+1)/2	GÉNÉRÉ PAR GeneSin.exe : NE PAS MODIFIER
#define RAYON	(0xFFFFFFFF/3/METRE)		// [0xFFFFFFFF = 3 m] Valeur max du sinus (= MAX_ULONG/3[m]/METRE)

extern T_dividedULong posX;					// [0xFFFFFFFF = 3 m]
extern T_dividedULong posY;					// [0xFFFFFFFF = 3 m]
extern unsigned short consigneX, consigneY; // [0xFFFF = 3 m] Consigne (X, Y)
extern unsigned long consPrecision;
// Alpha : Sens trigonométrique en vue de dessus
extern unsigned short posAlpha;				// [TOURieme de tour]
extern unsigned short alphaInit;			// [TOURieme de tour]	Init par PSoC
extern unsigned short posAlphaPrec;			// [TOURieme de tour]

void asser();

#endif

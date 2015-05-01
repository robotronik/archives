/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/

#ifndef _ASSER_H
#define _ASSER_H

#include "main.h"

#define TOUR	(int)6773 		// [tck/tr/roue] = 500*distance entre roue(=264mm)/diamètre roue(=70mm)
#define NB_SIN	(int)1694 		//  = 500*distance entre roue(=264mm)/diamètre roue(=70mm)/4
#define RAYON	(long)175375	// (val max du tableau = RAYON >> 2)

// Départ en 400,400 [mm] ; Max = 3 m
extern T_dividedULong posX;
extern T_dividedULong posY;
extern unsigned int consigneX, consigneY; 	// Consigne (X, Y) [0xFFFF = 3 m]
extern unsigned long consPrecision;
// Alpha : Sens trigonométrique en vue de dessus
extern unsigned int posAlpha;				// [TOURieme de tour]
extern unsigned int alphaInit;				// [TOURieme de tour]	Init par PSoC


void asser();

#endif

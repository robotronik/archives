#include "AlphaDelta.h"
#include "asser.h"
#include "messageAsser.h"
#include "main.h"
#include "motors.h"

long delta, alpha;
short vitDelta, vitAlpha;
long deltaPrec[TAILLE_TAB_VIT] = {0};	// [tck]
long alphaPrec[TAILLE_TAB_VIT] = {0};	// [TOURieme de tour]
unsigned short indexTab = 0;

inline void getVitMoy(void) {
	vitDelta = delta - deltaPrec[indexTab];	// [128eme de tck/période]
	vitAlpha = alpha - alphaPrec[indexTab];	// [TAILLE_TAB_VIT*TOURieme de tour/période]
    deltaPrec[indexTab] = delta;			// [demi-tck] 2 tck sur les roues -> 1 tck en delta
    alphaPrec[indexTab] = alpha;			// [TOURieme de tour]
}

inline void getDistArret(void) {
	sansFrein = (commandeCourante != FREINAGE) && nbCommande && (commande[indexCommande] != ALPHA);
	//dFreinageCarre = (unsigned long) mulSS(commandeDeltaPrec, commandeDeltaPrec) * coef_frein >> 15;	// [0xFFFF ~= 3 m]
	unsigned short commandeDeltaPrecAbs;
	if (commandeDeltaPrec < 0) commandeDeltaPrecAbs = -commandeDeltaPrec;
	else commandeDeltaPrecAbs = commandeDeltaPrec;
	dFreinageCarre = mulUU(commandeDeltaPrecAbs, coef_frein) >> 8;			// [0xFFFF ~= 3 m]
	dFreinageCarre *= (long) dFreinageCarre;
	
	/** Distance necessaire au freinage en Alpha **/
	aFreinage = (unsigned long) mulSS(commandeAlphaPrec, commandeAlphaPrec) * coef_frein_alpha >> 15;
}

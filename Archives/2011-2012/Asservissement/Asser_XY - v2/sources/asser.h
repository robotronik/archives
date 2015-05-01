#ifndef _ASSER_H
#define _ASSER_H

#include "Boolean.h"

// 1 tck = distance parcourue par une roue codeuse pour que le codeur s'incrémente

#define METRE		(short)22062			// [demi-tck] Informatif; = 2*1440[tcks]*1000[mm]/PI/diamètre roue(=41mm)
#define LARGEUR		324						// [mm] distance entre roues codeuses
#define COEFF_ALPH_DELT	2595                // [demi-tck / rad]=[demi-tck] = TOUR/(2*PI)	GÉNÉRÉ PAR GeneSin.exe : NE PAS MODIFIER, LAISSER DES ESPACES APRES
#define NB_SIN		(short)8154             // [tck/quart de tr] = (TOUR+1)/2				GÉNÉRÉ PAR GeneSin.exe : NE PAS MODIFIER, LAISSER DES ESPACES APRES
#define RAYON		(0xFFFFFFFF/3/METRE)	// [0xFFFFFFFF = 3 m] Valeur max du sinus (= MAX_ULONG/3[m]/METRE)
#define LONG_ARRIERE	36	// [mm]

inline void asser(void);
inline void chargeCommandeSuiv(void);
inline void fin(void);

extern unsigned short coef_frein;
extern unsigned short coef_frein_alpha;
extern unsigned short consAlpha;
extern unsigned short consigneX, consigneY;
extern unsigned long consPrecision;
extern unsigned char commandeCourante;
extern unsigned short posAlphaPrec;
extern long cosAlpha, sinAlpha;
extern unsigned long dFreinageCarre;
extern unsigned long aFreinage;
extern unsigned short alphaCibleDeplac;
extern short erreurAlphaDeplac, erreurAlphaDeplacMod;
extern long erreurX, erreurY;					// [0xFFFF = 3 m]
extern unsigned long erreurCarre;
extern short erreurAlpha;
extern boolean sansFrein, isErreurReduit;

#endif

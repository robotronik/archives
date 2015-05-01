#define TAILLE_TAB_VIT		64

long deltaPrec[TAILLE_TAB_VIT] = {0};	// [tck]
long alphaPrec[TAILLE_TAB_VIT] = {0};	// [TOURieme de tour]
short indexTab = 0;

#define getVitMoy																								\
	    /** Vitesses moyennes **/																				\
		vitDelta = delta - deltaPrec[indexTab];			/* [128eme de tck/période] */							\
		short vitAlpha = alpha - alphaPrec[indexTab];	/* [64*TOURieme de tour/période] */						\
	    deltaPrec[indexTab] = delta;					/* [demi-tck] 2 tck sur les roues -> 1 tck en delta */	\
	    alphaPrec[indexTab] = alpha;					/* [TOURieme de tour] */
		
#define getDistArret																			\
		/** Distance necessaire au freinage **/													\
		unsigned long dFreinageCarre;							/* [0xFFFF*0xFFFF ~= 3*3 m]	*/	\
		if ((commandeCourante != FREINAGE) && nbCommande && commande[indexCommande] & 0x40)		\
			sansFrein = TRUE;																	\
		else																					\
			sansFrein = FALSE;																	\
		/*if (sansFrein) dFreinageCarre = 0;*/													\
		/*else*/ {																				\
			long dFreinage = vitDelta * (long)coef_frein;		/* [0xFFFF ~= 3 m] */			\
			dFreinageCarre = dFreinage * dFreinage;												\
		}

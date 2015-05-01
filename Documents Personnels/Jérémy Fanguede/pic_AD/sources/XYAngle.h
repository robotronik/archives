// P  <-  R
//    X   |  alpha trigo 0° opposé à X
//      Y v

/*long distLPrec = 0;					// [tck]
long distRPrec = 0;*/					// [tck]

#define getXYAngle \
	    /** Distances totales **/																	\
		long distL, distR;				/* Position absolue de chaque roue [tck] */					\
	    motors_get_qei(&distL, &distR);	/* Recupere la distance totale parcourue par les roues */	\
	    																							\
	    /** Prise en compte des dérapages des roues folles **/										\
	    /*short vitL = distL - distLPrec;*/															\
	    /*short vitR = distR - distRPrec;*/															\
	    /*distLPrec = distL;*/																		\
	    /*distRPrec = distR;*/																		\
	    /*long vitAlphaCarre = vitL - vitR;*/														\
	    /*vitAlphaCarre *= vitAlphaCarre;*/															\
	    /*long vitLCarre = COEFF_ENTRAXE*vitAlphaCarre + vitL*vitL;*/								\
	    /*long vitRCarre = COEFF_ENTRAXE*vitAlphaCarre + vitR*vitR;*/								\
	    																							\
		long delta = distL + distR;																	\
		long alpha = distR - distL + alphaInit;														\
	    																							\
		/** Angle **/																				\
		if (alpha < 0) posAlpha = (alpha + 0xFFFFFFFF/TOUR*TOUR) % TOUR;							\
		else posAlpha = alpha % TOUR;																\
		unsigned short alphaDeplac;																	\
		if (posAlpha > posAlphaPrec + (TOUR >> 1) || posAlphaPrec > posAlpha + (TOUR >> 1))			\
			alphaDeplac = (posAlpha + posAlphaPrec + TOUR) % (TOUR << 1);							\
		else alphaDeplac = posAlpha + posAlphaPrec;													\
		posAlphaPrec = posAlpha;																	\
	    																							\
	    /** Vitesse instantanee **/																	\
		short vitDelta = delta - deltaPrec[indexTab];					/* / 1 periode */			\
	    																							\
	    /** posX, posY **/																			\
		long cosAlpha = myCos(alphaDeplac);															\
		long sinAlpha = mySin(alphaDeplac);															\
		posX.all -= vitDelta*cosAlpha;	/* * 1 periode */											\
		posY.all -= vitDelta*sinAlpha;																\
	    																							\
	    indexTab ++;																				\
	    indexTab %= TAILLE_TAB_VIT;							

#define getErreurs									\
	    /** Erreurs **/								\
		short erreurX, erreurY;	/* [0xFFFF = 3 m] */\
		short erreurAlpha;							\
		static short consMAlpha;					\
choice:	    										\
		erreurX = consigneX - posX.part.high;	    \
		erreurY = consigneY - posY.part.high;		\
		unsigned long erreurCarre = (unsigned long)((long)erreurY * erreurY) + (unsigned long)((long)erreurX * erreurX);	// [0xFFFF*0xFFFF = 3*3 m]

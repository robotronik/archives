#include "AlphaDelta.h"
#include "asser.h"
#include "messageAsser.h"
#include "motors.h"
#include "XYAngle.h"
	
inline void calle1(void) {
	/* Attend le démarrage */
	if (vitDelta < -5) commandeCourante += (short)CALLE_G2 - CALLE_G;
	consMDelta = -minSpeed-MAX_SPEED/5;
	consMAlpha = 0;
}

inline void calle2G(void) {
	if (!vitDelta) {
		posX.all = (long long)0xFFFFFFFF * (3000 - LONG_ARRIERE) / 3000;
		unsigned short alphaInitPrec = alphaInit;
		alphaInit = (TOUR + alphaInit - posAlpha) % TOUR;
		posAlphaPrec += (TOUR + alphaInit - alphaInitPrec) % TOUR;
		SEND_DONE;
		chargeCommandeSuiv();
	} else {
		consMDelta = -minSpeed-MAX_SPEED/5;
		consMAlpha = 0;
	}
}

inline void calle2H(void) {
	if (!vitDelta) {
		posY.all = (long long)0xFFFFFFFF * LONG_ARRIERE / 3000;
		unsigned short alphaInitPrec = alphaInit;
		alphaInit = (((unsigned long) TOUR*3/4) + TOUR + alphaInit - posAlpha) % TOUR;
		posAlphaPrec += (TOUR + alphaInit - alphaInitPrec) % TOUR;
		SEND_DONE;
		chargeCommandeSuiv();
	} else {
		consMDelta = -minSpeed-MAX_SPEED/5;
		consMAlpha = 0;
	}
}

inline void calle2D(void) {
	if (!vitDelta) {
		posX.all = (long long)0xFFFFFFFF * LONG_ARRIERE / 3000;
		unsigned short alphaInitPrec = alphaInit;
		alphaInit = ((unsigned long) (TOUR >> 1) + TOUR + alphaInit - posAlpha) % TOUR;
		posAlphaPrec += (TOUR + alphaInit - alphaInitPrec) % TOUR;
		SEND_DONE;
		chargeCommandeSuiv();
	} else {
		consMDelta = -minSpeed-MAX_SPEED/5;
		consMAlpha = 0;
	}
}

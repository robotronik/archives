#include "main.h"
#include "QEI.h"
#include "AlphaDelta.h"
#include "asser.h"
#include "myMath.h"
#include "debug.h"
#include "messageAsser.h"

#define X_START			LONG_ARRIERE
#define Y_START			250	// [mm]

T_dividedULong posX;					// [0xFFFFFFFF = 3 m]	Init dans main()
T_dividedULong posY;					// [0xFFFFFFFF = 3 m]	Init dans main()
unsigned short posAlpha = 0;			// [TOURieme de tour]
unsigned short alphaInit = 0;			// [TOURieme de tour]	Init par PSoC (dépendant de la couleur) ou par callage
	
inline void getXYAngle(void) {
	vitL = -distL; vitR = -distR;
    /** Distances totales **/
    getQEI(&distL, &distR);	// Recupere la distance totale parcourue par les roues
	vitL += distL; vitR += distR;
    
	delta = distL + distR;
	alpha = distR - distL + alphaInit;
	
	/** Angle **/
	if (alpha < 0) posAlpha = (alpha + 0xFFFFFFFF/TOUR*TOUR) % TOUR;
	else posAlpha = alpha % TOUR;
	static unsigned short alphaDeplac;
	if (posAlpha > posAlphaPrec + (TOUR >> 1) || posAlphaPrec > posAlpha + (TOUR >> 1))
		alphaDeplac = (posAlpha + posAlphaPrec + TOUR) % (TOUR << 1);
	else alphaDeplac = posAlpha + posAlphaPrec;
	posAlphaPrec = posAlpha;
    
    /** Vitesse instantanee **/
	vitDelta = delta - deltaPrec[indexTab];	// / 1 periode
	
    /** posX, posY **/
	cosAlpha = myCos(alphaDeplac);
	sinAlpha = mySin(alphaDeplac);
	posX.all -= vitDelta*cosAlpha;			// * 1 periode
	posY.all -= vitDelta*sinAlpha;
    
    indexTab ++;
    indexTab %= TAILLE_TAB_VIT;
}

inline void getErreurs(void) {
	erreurX = (long)consigneX - posX.part.high;
	erreurY = (long)consigneY - posY.part.high;
	/* Evite les débordements aux calculs d'erreurs² */
	if (erreurX > 46340 || erreurX < -46340) {
		erreurX >>= 1;	// -4 >> 1 = -2 testé pour C30
		erreurY >>= 1;
		isErreurReduit = TRUE;
	} else {
		isErreurReduit = FALSE;
	}
	
	if (erreurX > 0)	// alphaCibleDeplac = arctan(erreurY / erreurX)
		if (erreurY > 0)
			if (erreurX > erreurY)
				alphaCibleDeplac = (TOUR + (divS(mulSU(erreurY, TOUR), erreurX) >> 2)) >> 1;
			else
				alphaCibleDeplac = (3 * TOUR - (divS(mulSU(erreurX, TOUR), erreurY) >> 1)) >> 2;
		else
			if (erreurX > -erreurY)
				alphaCibleDeplac = (TOUR + (divS(mulSU(erreurY, TOUR), erreurX) >> 2)) >> 1;
			else
				alphaCibleDeplac = (TOUR - (divS(mulSU(erreurX, TOUR), erreurY) >> 1)) >> 2;
	else
		if (erreurY > 0)
			if (-erreurX > erreurY)
				alphaCibleDeplac = TOUR + (divS(mulSU(erreurY, TOUR), erreurX) >> 3);
			else
				alphaCibleDeplac = (3 * TOUR - (divS(mulSU(erreurX, TOUR), erreurY) >> 1)) >> 2;
		else
			if (-erreurX > -erreurY)
				alphaCibleDeplac = divS(mulSU(erreurY, TOUR), erreurX) >> 3;
			else
				if (erreurY == 0)
					alphaCibleDeplac = posAlpha;
				else
					alphaCibleDeplac = (TOUR - (divS(mulSU(erreurX, TOUR), erreurY) >> 1)) >> 2;
	SAVE_TIME(3);
	erreurAlphaDeplac = (TOUR - posAlpha + (unsigned long)alphaCibleDeplac) % TOUR;
	erreurAlphaDeplacMod = erreurAlphaDeplac % (TOUR / 2);
	
	erreurCarre = (unsigned long)(erreurY * erreurY) + (unsigned long)(erreurX * erreurX);
}

inline void redStart(void) {
	posX.all = (long long) 0xFFFFFFFF*X_START / 3000;
	posY.all = (long long) 0xFFFFFFFF*Y_START / 3000;
	alphaInit = TOUR >> 1;
	posAlphaPrec = alphaInit;
	
	commande[indexLibreCommande] = REGLAGE_CMD_MIN1;
	indexLibreCommande ++; indexLibreCommande %= NB_MAX_OCT_COMMANDES;
	nbCommande ++;
}

inline void purpleStart(void) {
	posX.all = (long long) 0xFFFFFFFF*(3000 - X_START) / 3000;
	posY.all = (long long) 0xFFFFFFFF*Y_START / 3000;
	alphaInit = 0;
	posAlphaPrec = alphaInit;
	
	commande[indexLibreCommande] = REGLAGE_CMD_MIN1;
	indexLibreCommande ++; indexLibreCommande %= NB_MAX_OCT_COMMANDES;
	nbCommande ++;
}

inline void sendPos(void) {
	UART_putc(GET_POS);
	UART_send_tab((unsigned char *) &posX.part.high, 2);
	UART_send_tab((unsigned char *) &posY.part.high, 2);
	UART_send_tab((unsigned char *) &posAlpha, 2);
}

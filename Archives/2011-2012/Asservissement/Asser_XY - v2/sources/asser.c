#include "messageAsser.h"
#include "motors.h"
#include "debug.h"
#include "main.h"
#include "XYAngle.h"
#include "AlphaDelta.h"
#include "asser.h"
#include "callage.h"

//#define TEST_MOTOR_MODEL

#define COEF_FREIN			2965// 2965 max ; Elevee -> Augmente dFreinage calcule -> Evite les oscillations à l'arrivee
#define COEF_FREIN_ALPHA	100	// 100 max ; Elevee -> Augmente aFreinage calcule -> Evite les oscillations à l'arrivee
#define ENTRAXE				(short)(20 * 53 * N / D)	// [tck] (= 53 mm)

unsigned short coef_frein = COEF_FREIN;
unsigned short coef_frein_alpha = COEF_FREIN_ALPHA;
unsigned short consigneX, consigneY;	// Consigne (X, Y) [0xFFFF = 3 m]
unsigned long  consPrecision;
unsigned short consAlpha = 0;			// [TOURieme de tour]
unsigned char  commandeCourante = FREINAGE;
unsigned short posAlphaPrec = 0;		// [TOURieme de tour]
long cosAlpha, sinAlpha;
/* dFreinageCarre: [0xFFFF*0xFFFF ~= (3*3) m²] Distance² necessaire au freinage en Delta */
unsigned long dFreinageCarre;
unsigned long aFreinage;
unsigned short alphaCibleDeplac;
short erreurAlphaDeplac, erreurAlphaDeplacMod;
long erreurX, erreurY;					// [0xFFFF = 3 m]
unsigned long erreurCarre;
short erreurAlpha;
boolean sansFrein = FALSE, isErreurReduit;
long rayon;								// [demi-tck / rad]=[demi-tck]
long diffX, diffY;
boolean reglage = FALSE;
boolean freinant = FALSE;

inline void asserXY(void);
inline void asserAlpha(void);

inline void fin(void) {
	motors_stop();
#if !defined(COLLECTEUR) && !defined(SIMU)
	while (1) FLASH_LONG;
#endif	// #if !defined(COLLECTEUR) && !defined(SIMU)
}

inline void asser(void) {
	SAVE_TIME(0);
	getXYAngle();	SAVE_TIME(0);
#ifndef TEST_MOTOR_MODEL
	getVitMoy();	SAVE_TIME(1);
	getDistArret();	SAVE_TIME(2);
	getErreurs();	SAVE_TIME(4);
	
	if (isPaused) freine();
	else {
		switch (commandeCourante) {
		case XY_RAPIDE :
		case XY_COURBE :
			asserXY();
			break;
		case ALPHA:
			asserAlpha();
			break;
		case FREINAGE :
			/** Consigne pour commande alpha **/
			consigneX = posX.part.high;
			consigneY = posY.part.high;
			/** Fin du freinage ? **/
			chargeCommandeSuiv();
			break;
		case CALLE_G:
		case CALLE_H:
		case CALLE_D:
			calle1();
			break;
		case CALLE_G2:
			calle2G();
			break;
		case CALLE_H2:
			calle2H();
			break;
		case CALLE_D2:
			calle2D();
			break;
		case REGLAGE_CMD_MIN1:
			reglage = TRUE;
			reglageCmdMin1();
			break;
		case REGLAGE_CMD_MIN2:
			reglageCmdMin2();
			break;
		case REGLAGE_CMD_MIN3:
			//reglage = FALSE;
			reglageCmdMin3();
		}
	}					SAVE_TIME(6);

	commandeMoteur(reglage);
						SAVE_TIME(7);
#else	// #ifndef TEST_MOTOR_MODEL

	static int ii = 0;
	if (ii > 1000) consMDelta = 0;
	else            consMDelta = 3999;
	ii ++;
	if (ii > 32000) ii = 0;
	consMAlpha = 0;
	motorsSetAlphaDeltaSpeed();
	
	// Activation de l'envoi de données
	commandeCourante = FREINAGE + 1;
	isDebugEnabled = TRUE;
#endif	// #ifndef TEST_MOTOR_MODEL
	
	debugProcess();
}

inline void asserXY(void) {
	/** Fin du déplacement ? **/
	if(erreurCarre < consPrecision && (sansFrein || (commandeDeltaPrec <= minSpeed && commandeDeltaPrec >= -minSpeed))) {
		SEND_DONE;
		chargeCommandeSuiv();
		return;
	}
	
	if (commandeCourante == XY_RAPIDE || !vitAlpha || (vitAlpha < 0 && consMAlpha > 0) || (vitAlpha > 0 && consMAlpha < 0)) {
		short erreurAlphaDeplacModTmp;
		
		/** Consigne en alpha **/
		if (erreurAlphaDeplacMod <= TOUR/4) {
			erreurAlphaDeplacModTmp = erreurAlphaDeplacMod;
			if (erreurAlphaDeplacModTmp > aFreinage)
				consMAlpha = divU(mulUU(erreurAlphaDeplacModTmp, MAX_SPEED - minSpeed) << 2, TOUR) + minSpeed;
			else consMAlpha = minSpeed;
		} else {
			erreurAlphaDeplacModTmp = (TOUR/2) - erreurAlphaDeplacMod;
			if (erreurAlphaDeplacModTmp > aFreinage)
				consMAlpha = -(long) divU(mulUU(erreurAlphaDeplacModTmp, MAX_SPEED - minSpeed) << 2, TOUR) - minSpeed;
			else consMAlpha = -minSpeed;
		}
		SAVE_TIME(5);
		consMAlpha = 0;
		
		/** Consigne en delta **/
		if (erreurCarre > dFreinageCarre && !freinant) {			// Rapide
		
			/** Sens de deplacement en delta **/
			if (((unsigned short)erreurAlphaDeplac + TOUR/4) % TOUR / (TOUR/2))		// Recule
				consMDelta = -(long) divU(mulUU(TOUR/4 - erreurAlphaDeplacModTmp, MAX_SPEED - minSpeed) << 2, TOUR) - minSpeed;
			else																	// Avance
				consMDelta =         divU(mulUU(TOUR/4 - erreurAlphaDeplacModTmp, MAX_SPEED - minSpeed) << 2, TOUR) + minSpeed;
		} else if (erreurAlphaDeplacMod < TOUR/8 && !freinant) {	// Lent
		
			freinant = TRUE;
			/** Sens de deplacement en delta **/
			if (((unsigned short)erreurAlphaDeplac + TOUR/4) % TOUR / (TOUR/2))		// Recule
				consMDelta = -minSpeed;
			else																	// Avance
				consMDelta = minSpeed;
		} else {consMDelta = 0;freinant = TRUE;}						// Arret
		
	} else {	/* Courbe : cas général */
		rayon = mulSU(vitDelta, COEFF_ALPH_DELT)/vitAlpha;	// [demi-tck / rad]=[demi-tck]
		if (isErreurReduit) rayon >>= 1;
		// UART_putc(vitDelta/vitAlpha);
		/* rayon > 0 : centre à gauche du robot */
		/* rayon < 0 : centre à droite du robot */
		// if (rayon < 0) rayon = -rayon;	// Commenté car le signe de rayon est utile pour les diffs
		diffX = erreurX - (long) ((sinAlpha*(long long)rayon) >> 16);
		diffY = erreurY + (long) ((cosAlpha*(long long)rayon) >> 16);
		rayon = (RAYON * (long long)rayon) >> 16;
		/* Si la cible est interne au cercle : tourner plus */
		if ((long long)diffX*diffX + (long long)diffY*diffY < (long long)rayon*rayon) {
			/*if (vitAlpha > 0)	consMAlpha =  MAX_SPEED >> 1;	// >> 1 pour consMDelta
			else				consMAlpha = -MAX_SPEED >> 1;	// >> 1 pour consMDelta*/
			if (vitAlpha > 0)	consMAlpha =  MAX_SPEED;
			else				consMAlpha = -MAX_SPEED;
		} else					consMAlpha =  0;
		SAVE_TIME(5);
		
		/** Consigne en delta **/
		if (erreurCarre > dFreinageCarre) {
		
			/** Sens de deplacement en delta **/
			if (((unsigned short)erreurAlphaDeplac + TOUR / 4) % TOUR / (TOUR / 2))
				if (consMAlpha < 0)	consMDelta = - consMAlpha - MAX_SPEED;	// Recule
				else				consMDelta =   consMAlpha - MAX_SPEED;
			else
				if (consMAlpha < 0)	consMDelta = MAX_SPEED + consMAlpha;	// Avance
				else				consMDelta = MAX_SPEED - consMAlpha;
		} else if (consMAlpha > -minSpeed && consMAlpha < minSpeed) {
		
			/** Sens de deplacement en delta **/
			if (((unsigned short)erreurAlphaDeplac + TOUR / 4) % TOUR / (TOUR / 2))
				if (consMAlpha < 0)	consMDelta = - consMAlpha - minSpeed;	// Recule
				else				consMDelta =   consMAlpha - minSpeed;
			else
				if (consMAlpha < 0)	consMDelta = minSpeed + consMAlpha;	// Avance
				else				consMDelta = minSpeed - consMAlpha;
		} else consMDelta = 0;
	}
}

inline void asserAlpha(void) {
	erreurAlpha = (TOUR - posAlpha + (unsigned long)consAlpha) % TOUR;
	/** Fin du déplacement ? **/
	if((erreurAlpha > TOUR-consPrecision || erreurAlpha < consPrecision) && (sansFrein || (commandeAlphaPrec <= minSpeed && commandeAlphaPrec >= -minSpeed))) {
		SEND_DONE;
		chargeCommandeSuiv();
		return;
	}
	
	/** Consigne en delta **/
	if (erreurCarre > dFreinageCarre) {
		
		/** Sens de deplacement en delta **/
		if (((unsigned short) erreurAlphaDeplac + TOUR/4) % TOUR / (TOUR/2))
			if (erreurAlphaDeplacMod <= TOUR/4)		// Recule
					consMDelta = -(long) mulUU(MAX_SPEED - minSpeed, (TOUR/4 - erreurAlphaDeplacMod) / (TOUR/4)) - minSpeed;
			else	consMDelta = -(long) mulUU(MAX_SPEED - minSpeed, (erreurAlphaDeplacMod - TOUR/4) / (TOUR/4)) - minSpeed;
		else
			if (erreurAlphaDeplacMod <= TOUR/4)		// Avance
					consMDelta =         mulUU(MAX_SPEED - minSpeed, (TOUR/4 - erreurAlphaDeplacMod) / (TOUR/4)) + minSpeed;
			else	consMDelta =         mulUU(MAX_SPEED - minSpeed, (erreurAlphaDeplacMod - TOUR/4) / (TOUR/4)) + minSpeed;
	} else consMDelta = 0;
	
	SAVE_TIME(5);
	/** Consigne en alpha **/
	if (erreurAlpha < TOUR/2) {
		if (erreurAlpha > aFreinage)
			if (consMDelta < 0)	consMAlpha =   MAX_SPEED + consMDelta;
			else				consMAlpha =   MAX_SPEED - consMDelta;
		else consMAlpha = minSpeed;
	} else {
		erreurAlpha = TOUR - erreurAlpha;
		if (erreurAlpha > aFreinage)
			if (consMDelta < 0)	consMAlpha = - MAX_SPEED - consMDelta;
			else				consMAlpha = - MAX_SPEED + consMDelta;
		else consMAlpha = -minSpeed;
	}
}

inline void chargeCommandeSuiv(void) {
	if (nbCommande) {
		nbCommande --;
		commandeCourante = commande[indexCommande];
		switch (commandeCourante) {
		case CALLE_G :
		case CALLE_H :
		case CALLE_D :
		case REGLAGE_CMD_MIN1 :
			break;
		case XY_RAPIDE :
		case XY_COURBE :
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consigneX = (unsigned short) commande[indexCommande] << 8;
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consigneX |= commande[indexCommande];
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consigneY = (unsigned short) commande[indexCommande] << 8;
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consigneY |= commande[indexCommande];
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consPrecision = (unsigned long) commande[indexCommande] << 24;
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consPrecision |= (unsigned long) commande[indexCommande] << 16;
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consPrecision |= (unsigned short) commande[indexCommande] << 8;
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consPrecision |= commande[indexCommande];
			break;
		case ALPHA :
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consAlpha = (unsigned short) commande[indexCommande] << 8;
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consAlpha |= commande[indexCommande];
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consPrecision = (unsigned short) commande[indexCommande] << 8;
			indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
			consPrecision |= commande[indexCommande];
		}
		indexCommande ++; indexCommande %= NB_MAX_OCT_COMMANDES;
	} else {
		commandeCourante = FREINAGE;
	}
	freine();
}

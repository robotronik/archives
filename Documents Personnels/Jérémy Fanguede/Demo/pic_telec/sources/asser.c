/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/

#include "main.h"
#include "myMath.h"
#include "motors.h"
#include "message.h"
#include "XYAngle.h"
#include "AlphaDelta.h"
#include "callage.h"

#ifdef SIMU
#include "prog2.h"
#endif

#define COEF_FREIN			35//(14000/TAILLE_TAB_VIT)//(ACCEL_MAX_ROUE << ?)	// Elevee -> Augmente dFreinage calcule -> Evite les oscillations à l'arrivee
#define COEF_FREIN_ALPHA	1//((150000/TAILLE_TAB_VIT)/TAILLE_TAB_VIT)			// Elevee -> Augmente aFreinage calcule -> Evite les oscillations
#define COEF_FREIN_ALPHA2	0//((150000/TAILLE_TAB_VIT)/TAILLE_TAB_VIT)			// Elevee -> Augmente aFreinage calcule -> Evite les oscillations
#define ENTRAXE				(short)(20 * 53 * N / D)							// [tck] (= 53 mm)

extern int isPaused;
extern unsigned char commandeSaved;
extern unsigned short consigneXSaved, consigneYSaved;

unsigned short coef_frein = COEF_FREIN;
unsigned short coef_frein_alpha = COEF_FREIN_ALPHA;
unsigned short coef_frein_alpha2 = COEF_FREIN_ALPHA2;
unsigned short min_speed = MIN_SPEED;

unsigned short consigneX = 0, consigneY = 0;	// Consigne (X, Y) [0xFFFF = 3 m]
unsigned long consPrecision;
unsigned short consAlpha = 0;					// [TOURieme de tour]
volatile boolean doitAttendre = TRUE;			// Echantilloneur
boolean sansFrein = FALSE;

unsigned char commandeCourante = FREINAGE;

#ifdef DATA
short cpt_boucle = 0;	// debug : compteur pour limiter les envois de données
#endif

void asser() {
	while(1)
 	{
#ifndef SIMU
		while (doitAttendre);			// Attend l'interruption du timer
		doitAttendre = TRUE;
#endif
		getXYAngle
		
		getVitMoy
		getDistArret
		
		getErreurs
		
		switch (commandeCourante) {
		case XY_RAPIDE :
		case XY_COURBE : {
			/** Fin du déplacement ? **/
			if(erreurCarre < consPrecision && (sansFrein || !vitDelta)) {
				UART_putc(DONE);
				commandeCourante = FREINAGE;
				goto choice;	// Retourne dans getErreurs
			}
			
			/** Vitesse en alpha **/
			if (commandeCourante == XY_RAPIDE || !vitAlpha || (vitAlpha < 0 && consMAlpha > 0) || (vitAlpha > 0 && consMAlpha < 0)) {
				unsigned short alphaCible;
				if (erreurX > 0)	// arctan
					if (erreurY > 0)
						if (erreurX > erreurY)
							alphaCible = (TOUR + ((short)((long) TOUR * erreurY / erreurX) >> 2)) >> 1;
						else
							alphaCible = (3 * TOUR - ((short)((long) TOUR * erreurX / erreurY) >> 1)) >> 2;
					else
						if (erreurX > -erreurY)
							alphaCible = (TOUR + ((short)((long) TOUR * erreurY / erreurX) >> 2)) >> 1;
						else
							alphaCible = (TOUR - ((short)((long) TOUR * erreurX / erreurY) >> 1)) >> 2;
				else
					if (erreurY > 0)
						if (-erreurX > erreurY)
							alphaCible = TOUR + ((short)((long) TOUR * erreurY / erreurX) >> 3);
						else
							alphaCible = (3 * TOUR - ((short)((long) TOUR * erreurX / erreurY) >> 1)) >> 2;
					else
						if (-erreurX > -erreurY)
							alphaCible = (short)((long) TOUR * erreurY / erreurX) >> 3;
						else
							alphaCible = (TOUR - ((short)((long) TOUR * erreurX / erreurY) >> 1)) >> 2;
				erreurAlpha = (TOUR - posAlpha + (unsigned long)alphaCible) % (TOUR / 2);
			
				/** Distance necessaire au freinage en Alpha **/
				unsigned long aFreinage = vitAlpha * vitAlpha * (unsigned long)coef_frein_alpha2;
				
				/** Consigne en alpha **/
				if (erreurAlpha <= TOUR/4) {
					if (erreurAlpha > aFreinage)
						consMAlpha = (long) 4 * erreurAlpha * MAX_SPEED / TOUR;
					else consMAlpha = 0;
				} else {
					erreurAlpha = TOUR/2 - erreurAlpha;
					if (erreurAlpha > aFreinage)
						consMAlpha = (long) -4 * erreurAlpha * MAX_SPEED / TOUR;
					else consMAlpha = 0;
				}
			} else {	// Courbe : cas général
				long rayon = (long) vitDelta*COEFF_ALPH_DELT/vitAlpha;	// [demi-tck / rad]=[demi-tck]
				//UART_putc(vitDelta/vitAlpha);
				// rayon > 0 : centre à gauche du robot
				// rayon < 0 : centre à droite du robot
				// if (rayon < 0) rayon = -rayon;	// Commenté car le signe de rayon est utile pour les diffs
				long diffX = erreurX - (long) ((sinAlpha*(long long)rayon) >> 16);
				long diffY = erreurY + (long) ((cosAlpha*(long long)rayon) >> 16);
				rayon = (RAYON * (long long)rayon) >> 16;
				// Si la cible est interne au cercle : tourner plus
				if ((long long)diffX*diffX + (long long)diffY*diffY < (long long)rayon*rayon) {
					if (vitAlpha > 0)	consMAlpha =  MAX_SPEED >> 1;	// >> 1 pour consMDelta
					else				consMAlpha = -MAX_SPEED >> 1;	// >> 1 pour consMDelta
				} else					consMAlpha =  0;
				debugCourbe
			}
			break;
			
		} case FREINAGE :
			/** Fin du déplacement ? **/
			if (!isPaused && nbCommande) chargeCommandeSuiv
			if (isPaused > 1) {		// Fin de pause
				commandeCourante = commandeSaved;
				consigneX = consigneXSaved;
				consigneY = consigneYSaved;
				isPaused = 0;
				goto choice;		// Retourne dans getErreurs
			}	
			consigneX = posX.part.high;
			consigneY = posY.part.high;
			dFreinageCarre = 0x7FFFFFFF;	// Oblige la suite a freiner
			consMAlpha = 0;
			break;
			
		case CALLAGE
			
		default : {		// Asser alpha
			erreurAlpha = (TOUR - posAlpha + (unsigned long)consAlpha) % TOUR;
			/** Fin du déplacement ? **/
			if((erreurAlpha > TOUR-consPrecision || erreurAlpha < consPrecision) && (sansFrein || !vitAlpha)) {
				UART_putc(DONE);
				commandeCourante = FREINAGE;
				goto choice;	// Retourne dans getErreurs
			}
			
			/** Distance necessaire au freinage en Alpha **/
			unsigned long aFreinage = vitAlpha * vitAlpha * (unsigned long)coef_frein_alpha;
			
			/** Consigne en alpha **/
			if (erreurAlpha < TOUR/2) {
				if (erreurAlpha > aFreinage)
					consMAlpha = MAX_SPEED >> 1;	// >> 1 pour corrections delta
				else consMAlpha = 0;
			} else {
				erreurAlpha = TOUR - erreurAlpha;
				if (erreurAlpha > aFreinage)
					consMAlpha = -MAX_SPEED >> 1;	// >> 1 pour corrections delta
				else consMAlpha = 0;
			}
		}}
	
		/** Consigne en delta **/
		short consMDelta;
		if (erreurCarre > dFreinageCarre) {
		
			/** Sens de deplacement en delta **/
			erreurX += cosAlpha >> 8;	// >> 8 empèche le débordement si la longueur du robot est supérieur à 11 mm
			erreurY += sinAlpha >> 8;
			if ((unsigned long)((long)erreurY * erreurY) + (unsigned long)((long)erreurX * erreurX) < erreurCarre)
				if (consMAlpha < 0)	consMDelta = MAX_SPEED + consMAlpha;	// Avance
				else				consMDelta = MAX_SPEED - consMAlpha;
			else															// Recule
				if (consMAlpha < 0)	consMDelta = - consMAlpha - MAX_SPEED;
				else				consMDelta =   consMAlpha - MAX_SPEED;
		} else consMDelta = 0;
		
		commandeMoteur
#ifdef DATA
		debug
#endif
		if (!doitAttendre) allumer_del();	// Timer trop rapide : allume LED debug
	}
}

#ifndef SIMU
void __attribute__((interrupt, auto_psv)) _MPWM1Interrupt(void) {
	IFS3bits.PWM1IF = 0;	// On s'acquitte de l'interruption
	doitAttendre = FALSE;
}
#endif

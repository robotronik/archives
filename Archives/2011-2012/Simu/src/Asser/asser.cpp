/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/

#include "main.h"
#include "myMath.h"
#include "motors.h"
#include "message.h"

/*
#ifdef SIMU
#define DATA 800	// [per] Période d'envoie de donnees
#else
#define DATA 40		// [per] Période d'envoie de donnees
#endif //*/

#include "prog2.h"
#include "redef.h"

#define TAILLE_TAB_VIT		64
#define ACCEL_MAX_ROUE_INT	(int)((long)200000 * D / N / ASSER_PERIOD / ASSER_PERIOD)
#define ACCEL_MAX_ROUE		2//(ACCEL_MAX_ROUE_INT >> 9)								// [periode²/512ème de tck] Temps entre chaque accélération de 1 = 1/(0,1 m/s²) (= 7)
//#define COEF_FREIN			29500/TAILLE_TAB_VIT//(ACCEL_MAX_ROUE << ?)					// Elevee -> Augmente dFreinage calcule -> Evite les oscillations à l'arrivee
#define COEF_FREIN			14000/TAILLE_TAB_VIT//(ACCEL_MAX_ROUE << ?)					// Elevee -> Augmente dFreinage calcule -> Evite les oscillations à l'arrivee
#define COEF_FREIN_ALPHA	7000/TAILLE_TAB_VIT/TAILLE_TAB_VIT							// Elevee -> Augmente aFreinage calcule -> Evite les oscillations
#define COEF_FREIN_ALPHA2	15000/TAILLE_TAB_VIT/TAILLE_TAB_VIT							// Elevee -> Augmente aFreinage calcule -> Evite les oscillations
#define DECAL_ACCEL			1															// 2^DECAL_ACCEL = ACCEL_MAX_ROUE

// Départ en 400,400 [mm] ; Max = 3 m
T_dividedULong posX = {0};
T_dividedULong posY = {0};
unsigned int consigneX = 0, consigneY = 0; 	// Consigne (X, Y) [0xFFFF = 3 m]
unsigned long consPrecision;
// Alpha : Sens trigonométrique en vue de dessus
unsigned int posAlpha = 0;			// [TOURieme de tour]
unsigned int alphaInit;				// [TOURieme de tour]	Init par PSoC
unsigned int consAlpha;				// [TOURieme de tour]
boolean doitAttendre = TRUE;		// Echantilloneur
boolean sansFrein = FALSE;
int consR = 0, consL = 0;			// Commande des moteurs a l'anti-bloquage pres
long deltaPrec[TAILLE_TAB_VIT] = {0};	// [tck]
long alphaPrec[TAILLE_TAB_VIT] = {0};	// [tck]
int indexTab = 0;
int tempo_accel = 0;

unsigned char commandeCourante = FREINAGE;

#ifdef DATA
int cpt_boucle = 0;	// debug : timer pour limiter les envois de données
#endif

void asser() {
	while(1)
 	{
	    /** Distances totales **/
		long distL, distR;				// Position absolue de chaque roue [tck]
	    motors_get_qei(&distL, &distR);	// Recupere la distance totale parcourue par les roues
	    distL >>= 5;
	    distR >>= 5;
		long delta = distL + distR;
		long alpha = distR - distL + alphaInit;

	    /** Vitesse instantanee **/
		int vitDelta = delta - deltaPrec[indexTab];
	    indexTab ++;
	    indexTab %= TAILLE_TAB_VIT;

	    /** posX, posY **/
		long cosAlpha = myCos(posAlpha);
		long sinAlpha = mySin(posAlpha);
		posX.all += vitDelta*cosAlpha;
		posY.all -= vitDelta*sinAlpha;

		/** Angle **/
		if (alpha < 0) posAlpha = (alpha + 2147481245) % TOUR;
		else posAlpha = alpha % TOUR;

	    /** Vitesses moyennes **/
		vitDelta = delta - deltaPrec[indexTab];					// [128eme de tck/période]
		int vitAlpha = alpha - alphaPrec[indexTab];				// [64*TOURieme de tour/période]
	    deltaPrec[indexTab] = delta;							// [demi-tck]
	    alphaPrec[indexTab] = alpha;							// [TOURieme de tour]

		/** Distance necessaire au freinage **/
		unsigned long dFreinageCarre;							// [0xFFFF*0xFFFF ~= 3*3 m]
		if ((commandeCourante != FREINAGE) && nbCommande && commande[indexCommande] & 0x40)
			sansFrein = TRUE;
		else
			sansFrein = FALSE;
		/*if (sansFrein) dFreinageCarre = 0;
		else*/ {
			long dFreinage = vitDelta * (long)COEF_FREIN;		// [0xFFFF ~= 3 m]
			dFreinageCarre = dFreinage * dFreinage;
		}

	    /** Erreurs **/
		int erreurX, erreurY;									// [0xFFFF = 3 m]
		int consMAlpha, erreurAlpha;
choice:
		erreurX = consigneX - posX.part.high;
		erreurY = consigneY - posY.part.high;
		unsigned long erreurCarre = (unsigned long)((long)erreurY * erreurY) + (unsigned long)((long)erreurX * erreurX);	// [0xFFFF*0xFFFF = 3*3 m]

		switch (commandeCourante) {
		case XY_RAPIDE :
		case XY_COURBE : {
			/** Fin du déplacement ? **/
			if(erreurCarre < consPrecision && (sansFrein || !vitDelta)) {
				UART_putc(DONE);
				commandeCourante = FREINAGE;
				goto choice;
			}

			/** Vitesse en alpha **/
			if (!vitAlpha || commandeCourante == XY_RAPIDE) {
				unsigned int alphaCible;
				if (erreurX > 0)
					if (erreurY > 0)
						if (erreurX > erreurY)
							alphaCible = (TOUR + ((int)((long) TOUR * erreurY / erreurX) >> 2)) >> 1;
						else
							alphaCible = (3 * TOUR - ((int)((long) TOUR * erreurX / erreurY) >> 1)) >> 2;
					else
						if (erreurX > -erreurY)
							alphaCible = (TOUR + ((int)((long) TOUR * erreurY / erreurX) >> 2)) >> 1;
						else
							alphaCible = (TOUR - ((int)((long) TOUR * erreurX / erreurY) >> 1)) >> 2;
				else
					if (erreurY > 0)
						if (-erreurX > erreurY)
							alphaCible = TOUR + ((int)((long) TOUR * erreurY / erreurX) >> 3);
						else
							alphaCible = (3 * TOUR - ((int)((long) TOUR * erreurX / erreurY) >> 1)) >> 2;
					else
						if (-erreurX > -erreurY)
							alphaCible = (int)((long) TOUR * erreurY / erreurX) >> 3;
						else
							alphaCible = (TOUR - ((int)((long) TOUR * erreurX / erreurY) >> 1)) >> 2;
				// if (cpt_boucle == 0) send_message((long)posAlpha * 360 / TOUR);
				// if (cpt_boucle == 0) send_message((long)alphaCible * 360 / TOUR);
				erreurAlpha = (alphaCible - posAlpha + TOUR) % (TOUR / 2);

				/** Distance necessaire au freinage en Alpha **/
				unsigned long aFreinage = vitAlpha * vitAlpha * (unsigned long)COEF_FREIN_ALPHA2;

				/** Consigne en alpha **/
				if (erreurAlpha <= TOUR/4) {
					if (erreurAlpha > aFreinage)
						consMAlpha = (long) 4 * erreurAlpha * MAX_SPEED / TOUR;
						//consMAlpha = MAX_SPEED >> 1;			// >> 1 pour corrections delta
					else consMAlpha = 0;
				} else {
					erreurAlpha = TOUR/2 - erreurAlpha;
					if (erreurAlpha > aFreinage)
						consMAlpha = (long) -4 * erreurAlpha * MAX_SPEED / TOUR;
						//consMAlpha = -MAX_SPEED >> 1;			// >> 1 pour corrections delta
					else consMAlpha = 0;
				}
			} else if (vitDelta) {
				long rayon = (long) vitDelta*1078/vitAlpha;		// [demi-tck / rad]=[demi-tck] 1078 = TOUR/(2*PI)
				long diffX = erreurX - (sinAlpha*(long long)rayon >> 16);
				long diffY = erreurY - (cosAlpha*(long long)rayon >> 16);
				rayon = RAYON * (long long)rayon >> 16;
				// Si consigne interne au cercle : tourner plus
				if ((long long)diffX*diffX + (long long)diffY*diffY < (long long)rayon*rayon) {
					if (vitAlpha > 0)	consMAlpha = MAX_SPEED >> 1;	// >> 1 pour corrections delta
					else				consMAlpha = -MAX_SPEED >> 1;	// >> 1 pour corrections delta
				} else 					consMAlpha = 0;
			}
			break;

		} case FREINAGE :
			/** Fin du déplacement ? **/
			if (nbCommande) {
				nbCommande --;
				commandeCourante = commande[indexCommande];
				switch (commandeCourante) {
				case XY_RAPIDE :
				case XY_COURBE :
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consigneX = (unsigned int) commande[indexCommande] << 8;
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consigneX |= commande[indexCommande];
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consigneY = (unsigned int) commande[indexCommande] << 8;
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consigneY |= commande[indexCommande];
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consPrecision = (unsigned long) commande[indexCommande] << 24;
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consPrecision |= (unsigned long) commande[indexCommande] << 16;
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consPrecision |= (unsigned int) commande[indexCommande] << 8;
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consPrecision |= commande[indexCommande];
					break;
				default :	// Asser alpha
					consAlpha = (unsigned int) commande[indexCommande] << 8;
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consAlpha |= commande[indexCommande];
					indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
					consPrecision = commande[indexCommande];
				}
				indexCommande ++; indexCommande %= NB_MAX_COMMANDES;
				goto choice;
			}
			consigneX = posX.part.high;
			consigneY = posY.part.high;
			dFreinageCarre = 2147483647;	// Oblige a freiner
			consMAlpha = 0;
			break;

		case CALLE_G:
		case CALLE_H:
		case CALLE_D:
			if (vitDelta) commandeCourante += 3;
			motors_set_speed(70, 70);
			return;

		case CALLE_G2:
			if (!vitDelta) {
				//posX = PROFONDEUR/2;
				posAlpha = 0;
				motors_stop();
				commandeCourante = FREINAGE;
			}
			return;

		case CALLE_H2:
			if (!vitDelta) {
				//posY = PROFONDEUR/2;
				posAlpha = TOUR*3 >> 2;
				motors_stop();
				commandeCourante = FREINAGE;
			}
			return;

		case CALLE_D2:
			if (!vitDelta) {
				//posX = 0xFFFFFFFF - PROFONDEUR/2;
				posAlpha = TOUR >> 1;
				motors_stop();
				commandeCourante = FREINAGE;
			}
			return;

		default : {		// Asser alpha
			erreurAlpha = (consAlpha - posAlpha + TOUR) % TOUR;
			/** Fin du déplacement ? **/
			if((erreurAlpha > TOUR-consPrecision || erreurAlpha < consPrecision) && (sansFrein || !vitAlpha)) {
				UART_putc(DONE);
				commandeCourante = FREINAGE;
				goto choice;
			}

			/** Distance necessaire au freinage en Alpha **/
			unsigned long aFreinage = vitAlpha * vitAlpha * (unsigned long)COEF_FREIN_ALPHA;

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
		int consDelta;
		if (erreurCarre > dFreinageCarre) {

			/** Sens de deplacement en delta **/
			if (cosAlpha >= 1) erreurX --;
			else if (cosAlpha <= -1) erreurX ++;
			if (sinAlpha >= 1) erreurY ++;
			else if (sinAlpha <= -1) erreurY --;
			if ((unsigned long)((long)erreurY * erreurY) + (unsigned long)((long)erreurX * erreurX) < erreurCarre)
				consDelta = MAX_SPEED - consMAlpha;
			else
				consDelta = consMAlpha - MAX_SPEED;
		} else consDelta = 0;

		/** (delta, alpha) -> (R, L) **/
		/** Limitation de l'accélération **/
		static int cumulConsMAlpha = 0, cumulConsDelta = 0;
		cumulConsMAlpha += consMAlpha;
		cumulConsDelta += consDelta;
		int commandeL, commandeR;
		if (!tempo_accel) {
			int commandeAlphaPrec = consR - consL, commandeDeltaPrec = consR + consL;
			if (cumulConsMAlpha >= commandeAlphaPrec) {
				if (cumulConsDelta >= commandeDeltaPrec)
					consR ++;
				if (cumulConsDelta <= commandeDeltaPrec)
					consL --;
			} if (cumulConsMAlpha <= commandeAlphaPrec) {
				if (cumulConsDelta >= commandeDeltaPrec)
					consL ++;
				if (cumulConsDelta <= commandeDeltaPrec)
					consR --;
			}
			cumulConsMAlpha = 0;
			cumulConsDelta = 0;

			/** Evite le bloquage des roues */
			if(consL && consL > -10 && consL < 70) {	// Arrêt des moteurs à +53 et -0
				if (consL > 0) commandeL = 70;
				else commandeL = -10;
			} else commandeL = consL;
			if(consR && consR > -10 && consR < 70) {	// Arrêt des moteurs à +53 et -0
				if (consR > 0) commandeR = 70;
				else commandeR = -10;
			} else commandeR = consR; //*/

			/** Envoi des commandes aux moteurs **/
			motors_set_speed(commandeL, commandeR);
		}
		tempo_accel ++;
		tempo_accel %= ACCEL_MAX_ROUE;
        return;

#ifdef DATA
		// if (cpt_boucle == 0) send_message(distL);
		// if (cpt_boucle == 0) send_message(distR);
		// if (cpt_boucle == 0) send_message(vitAlpha*vitAlpha*COEF_FREIN_ALPHA);
		// if (cpt_boucle == 0) send_message((unsigned long)posX.part.high*3000/0xFFFF);
		// if (cpt_boucle == 0) send_message((unsigned long)posY.part.high*3000/0xFFFF);
		// if (cpt_boucle == 0) send_message((long)posAlpha*360/TOUR);
		// if (cpt_boucle == 0) send_message(nbCommande);
		// if (cpt_boucle == 0) send_message(commandeCourante);
		// if (cpt_boucle == 0) send_message(consPrecision);
		// if (cpt_boucle == 0) send_message((unsigned long)consigneX*3000/0xFFFF);
		// if (cpt_boucle == 0) send_message((unsigned long)consigneY*3000/0xFFFF);
		// if (cpt_boucle == 0) send_message(vitAlpha);
		// if (cpt_boucle == 0) send_message(vitDelta);
//		if (cpt_boucle == 0) send_message(sinAlpha);
		// if (cpt_boucle == 0) send_message(erreurX);
//		if (cpt_boucle == 0) send_message(cosAlpha);
		// if (cpt_boucle == 0) send_message(erreurY);
		// if (cpt_boucle == 0) send_message(erreurX*3000/0xFFFF);
		// if (cpt_boucle == 10) send_message(erreurY*3000/0xFFFF);
		// if (cpt_boucle == 0) send_message(erreurCarre);
		// if (cpt_boucle == 0) send_message(dFreinageCarre);
		// if (cpt_boucle == 0) send_message(erreurCarre - dFreinageCarre);
		// if (cpt_boucle == 0) send_message(consMAlpha);
		// if (cpt_boucle == 0) send_message(consDelta);
		// if (cpt_boucle == 0) send_message(consR);
		// if (cpt_boucle == 0) send_message(commandeL);
		// if (cpt_boucle == 0) send_message(commandeR);
		// if (cpt_boucle == 0) send_message(commandeR + commandeL);
		// if (cpt_boucle == 0) send_message(commandeR - commandeL);

		// if (cpt_boucle == 0) pause(1);
		cpt_boucle ++;
		cpt_boucle %= DATA;
#endif
		if (!doitAttendre) allumer_del();	// Timer trop rapide : allume LED debug
	}
}


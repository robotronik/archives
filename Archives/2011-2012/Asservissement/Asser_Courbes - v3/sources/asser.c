/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/


#include "main.h"

#define VITESSE_MAX_ROUE	(int)(((long)2 * N * ASSER_PERIOD << 9) / D / 5)	// [512ème de tck/période] Vitesse max d'une roue = 0,4 m/s (= 5349)
#define ACCEL_MAX_ROUE_INT	(int)((long)200000 * D / N / ASSER_PERIOD / ASSER_PERIOD)
#define ACCEL_MAX_ROUE		ACCEL_MAX_ROUE_INT >> 9								// [période²/512ème de tck] Temps entre chaque accélération de 1 = 1/(0,1 m/s²) (= 7)
#define COEF_FREIN			ACCEL_MAX_ROUE << 4								// Elevée -> Augmente dFreinage calculé -> Evite les oscillations à l'arrivée

#define ALPHA_CONS 1			// >> 0 ensuite [% * 40 * période / 512ème de tck]
#define ALPHA_P 40				// >> 0 ensuite [% * 40 / tck]
#define ALPHA_I 4				// >> 6 ensuite [% * 40 / tck]
#define DELTA_CONS ALPHA_CONS	// >> 0 ensuite [% * 40 * période / 512ème de tck]
#define DELTA_P 40				// >> 0 ensuite [% * 40 / tck]
#define DELTA_I 4				// >> 6 ensuite [% * 40 / tck]


long consigneAlpha, consigneDelta; 	// Consigne (alpha, delta)
int rapportVitesse;					// Vitesse angulaire/vitesse linéaire ou inverse (>= -1 et < 1) << 15
T_dividedLong distL, distR;			// Position de chaque roue pour un trajet
int doitAttendre = TRUE;			// Echantilloneur


void asser_delta() {	// Boucle d'asser en delta
	long deltaMes = 0, dFreinage, tabDeltaMes[4] = {0, 0, 0, 0};
	long alphaMes = 0, tabAlphaMes[4] = {0, 0, 0, 0};	// Sens trigonométrique en vue de dessus
	int indexTab = 0;
	int vitesseMaxDelta, vitesseDelta, vitesseAlpha;
	int consigneAccelDelta, consigneVitesseDelta = 0;
	int tempo_accel = 0;
	long erreurDelta, erreurDeltaInterne = 0, erreurAlpha;
	long long integralErreurAlpha = 0;
	long long integralErreurDelta = 0;
	int commandeDelta, commandeAlpha;
	int commandeL, commandeR;

	static int cpt_boucle = 0;	// debug : timer pour limiter les envois de données

	vitesseMaxDelta = (VITESSE_MAX_ROUE - (int)((VITESSE_MAX_ROUE * (long)rapportVitesse) >> 16)) >> 5;	// [quart de tck/période]
	//accelMaxDelta = 1/(1/ACCEL_MAX_ROUE - (int)(1/ACCEL_MAX_ROUE * (long)rapportVitesse >> 16));
	
	while(1)
 	{
		while (doitAttendre);			// Attend l'interruption du timer
		doitAttendre = TRUE;
		
	    /** Actualisation position actuelle (distL, distR) **/
	    distL.part.low = motors_get_qei(LEFT);	// Recupere la distance totale parcourue par une roue
	    distR.part.low = motors_get_qei(RIGHT);	// distX.part.high est mis à jour par interruption
	
	    /** Changement variable (distL, distR) -> (deltaMes, alphaMes) **/
	    deltaMes = distR.all + distL.all;							// [tck]
		//if (!cpt_boucle) send_message(deltaMes);
	    alphaMes = distR.all - distL.all;							// [tck]
		//if (!cpt_boucle) send_message(alphaMes);
	    
	    /** Actualisation vitesses moyennes sur 4 échantillons **/
	    vitesseDelta = deltaMes - tabDeltaMes[indexTab];			// [quart de tck/période]
		//if (cpt_boucle == 10) send_message(vitesseDelta);
	    tabDeltaMes[indexTab] = deltaMes;							// [tck]
	    vitesseAlpha = alphaMes - tabAlphaMes[indexTab];			// [quart de tck/période]
		//if (cpt_boucle == 30) send_message(vitesseAlpha);
	    tabAlphaMes[indexTab] = alphaMes;							// [tck]
	    indexTab ++;
	    indexTab %= 4;
	
		//if (!cpt_boucle) send_message(consigneDelta);
		erreurDelta = consigneDelta - deltaMes;
		//if (!cpt_boucle) send_message(erreurDelta);
		erreurAlpha = (long)((long long)deltaMes * rapportVitesse >> 15) - alphaMes;
		if (cpt_boucle == 0) send_message(erreurAlpha);
	
		/** Test de fin du déplacement **/
		if(!vitesseDelta && erreurDelta<26/* && !vitesseAlpha && erreurAlpha<26*/) {
			motors_stop();
			motors_reset_qei(LEFT);
			motors_reset_qei(RIGHT);
			//send_message(DONE);
			consigneDelta = 0;
			/*consigneAlpha = erreurAlpha;
			rapportVitesse = 0;
			asser_alpha();*/
			break;
		}
		
		/** Calcul de la distance necessaire au freinage **/
		dFreinage = (long) vitesseDelta * vitesseDelta * COEF_FREIN;
		//if (cpt_boucle == 10) send_message(dFreinage);
	
		/** Choix de la vitesse à atteindre et calcul de l'accélération **/
		if (erreurDelta <= dFreinage && erreurDelta >= -dFreinage)
			consigneAccelDelta = -vitesseDelta;						// Doit s'arreter (vitesseDelta -> 0)
		else if (erreurDelta < 0)
			consigneAccelDelta = -vitesseMaxDelta - vitesseDelta;	// Doit reculer (vitesseDelta -> -vitesseMaxDelta)
		else consigneAccelDelta = vitesseMaxDelta - vitesseDelta;	// Doit avancer (vitesseDelta -> vitesseMaxDelta)
		
		/** Limitation de l'accélération **/
		if (tempo_accel) consigneAccelDelta = 0;
		else if (consigneAccelDelta > 1) consigneAccelDelta = 1;
		else if (consigneAccelDelta < -1) consigneAccelDelta = -1;
		tempo_accel ++;
		tempo_accel %= ACCEL_MAX_ROUE;
		//if (cpt_boucle == 20) send_message(consigneAccelDelta);
	
		/** PI(D) Delta **/
		erreurDeltaInterne += consigneVitesseDelta - (vitesseDelta << 5);	// [512ème de tck]
		//if (cpt_boucle == 20) send_message(erreurDeltaInterne);
		integralErreurDelta += erreurDeltaInterne;
		consigneVitesseDelta += consigneAccelDelta;							// [512ème de tck / période]
		//if (cpt_boucle == 30) send_message(consigneVitesseDelta);
		commandeDelta = DELTA_CONS * consigneVitesseDelta + ((DELTA_P * erreurDeltaInterne) >> 9) + ((DELTA_I * integralErreurDelta) >> 15);
		//if (cpt_boucle == 10) send_message(commandeDelta);
	
		/** PI(D) Alpha **/
		integralErreurAlpha += erreurAlpha;
		commandeAlpha = (long)((long long)commandeDelta * rapportVitesse >> 15) + ALPHA_P * erreurAlpha + ((ALPHA_I * integralErreurAlpha) >> 6);
		//if (cpt_boucle == 0) send_message(commandeAlpha);
	    
		/** Changement de variable (commandeDelta, commandeAlpha) -> (commandeR, commandeL) **/
	    commandeR = commandeDelta + commandeAlpha;
	    commandeL = commandeDelta - commandeAlpha;
	
		/** Evite le bloquage des roues /
		if(commandeR && commandeR > -10 && commandeR < 70) {	// Arrêt des moteurs à +53 et -0
			if (commandeR > 0) commandeR = 70;
			else commandeR = -10;
		}
		if(commandeL && commandeL > -10 && commandeL < 70) {	// Arrêt des moteurs à +53 et -0
			if (commandeL > 0) commandeL = 70;
			else commandeL = -10;
		} //*/
		//if (cpt_boucle == 0) send_message(commandeR);
		//if (cpt_boucle == 5) send_message(commandeL);
	
		/** Détection des débordements sur le moteur gauche **/
		//if (cpt_boucle == 10 && (commandeL < -MAX_SPEED || commandeL > MAX_SPEED)) send_message(0x11111111);
		//if (cpt_boucle == 10 && (commandeL < -MAX_SPEED || commandeL > MAX_SPEED)) send_message(erreurAlpha);
		/** Détection des débordements sur le moteur droit **/
		//if (cpt_boucle == 20 && (commandeR < -MAX_SPEED || commandeR > MAX_SPEED)) send_message(0x22222222);
		//if (cpt_boucle == 10 && (commandeR < -MAX_SPEED || commandeR > MAX_SPEED)) send_message(erreurAlpha);
	
	    /** Envoi des commandes aux moteurs **/
	    motors_set_speed(commandeL, commandeR);
		
		//*
		cpt_boucle ++;
		cpt_boucle %= 10; // Règle la fréquence d'envoie */
		if (!doitAttendre) _RB5 = 0;	// Timer trop rapide : allume LED debug
	}
}

void asser_alpha() {	// Boucle d'asser en alpha
	long alphaMes = 0, aFreinage, tabAlphaMes[4] = {0, 0, 0, 0};	// Sens trigonométrique en vue de dessus
	long deltaMes = 0, tabDeltaMes[4] = {0, 0, 0, 0};
	int indexTab = 0;
	int vitesseMaxAlpha, vitesseDelta, vitesseAlpha;
	int consigneAccelAlpha, consigneVitesseAlpha = 0;
	int tempo_accel = 0;
	long erreurAlpha, erreurAlphaInterne = 0, erreurDelta;
	long long integralErreurDelta = 0;
	long long integralErreurAlpha = 0;
	int commandeDelta, commandeAlpha;
	int commandeL, commandeR;

	static int cpt_boucle = 0;	// debug : timer pour limiter les envois de données

	vitesseMaxAlpha = (VITESSE_MAX_ROUE - (int)((VITESSE_MAX_ROUE * (long)rapportVitesse) >> 16)) >> 5;	// [tck/période]
	//accelMaxDelta = 1/(1/ACCEL_MAX_ROUE - (int)(1/ACCEL_MAX_ROUE * (long)rapportVitesse >> 16));
	
	while(1)
 	{
		while (doitAttendre);			// Attend l'interruption du timer
		doitAttendre = TRUE;
		
	    /** Actualisation position actuelle (distL, distR) **/
	    distL.part.low = motors_get_qei(LEFT);	// Recupere la distance totale parcourue par une roue
	    distR.part.low = motors_get_qei(RIGHT);	// distX.part.high est mis à jour par interruption
	
	    /** Changement variable (distL, distR) -> (deltaMes, alphaMes) **/
	    deltaMes = distR.all + distL.all;							// [tck]
		//if (!cpt_boucle) send_message(deltaMes);
	    alphaMes = distR.all - distL.all;							// [tck]
		//if (!cpt_boucle) send_message(alphaMes);
	    
	    /** Actualisation vitesses moyennes sur 4 échantillons **/
	    vitesseDelta = deltaMes - tabDeltaMes[indexTab];			// [quart de tck/période]
		//if (cpt_boucle == 10) send_message(vitesseDelta);
	    tabDeltaMes[indexTab] = deltaMes;							// [tck]
	    vitesseAlpha = alphaMes - tabAlphaMes[indexTab];			// [quart de tck/période]
		//if (cpt_boucle == 30) send_message(vitesseAlpha);
	    tabAlphaMes[indexTab] = alphaMes;							// [tck]
	    indexTab ++;
	    indexTab %= 4;
	
		//if (!cpt_boucle) send_message(consigneAlpha);
		erreurAlpha = consigneAlpha - alphaMes;
		//if (!cpt_boucle) send_message(erreurAlpha);
		erreurDelta = (long)((long long)alphaMes * rapportVitesse >> 15) - deltaMes;
		//if (!cpt_boucle) send_message(erreurDelta);
	
		/** Test de fin du déplacement **/
		if(!vitesseDelta && erreurDelta<26 && !vitesseAlpha && erreurAlpha<26) {
			motors_stop();
			motors_reset_qei(LEFT);
			motors_reset_qei(RIGHT);
			//send_message(DONE);
			consigneAlpha = 0;
			break;
		}
		
		/** Calcul de la distance necessaire au freinage **/
		aFreinage = (long) vitesseAlpha * vitesseAlpha * COEF_FREIN;
		//if (!cpt_boucle) send_message(aFreinage);
	
		/** Choix de la vitesse à atteindre et calcul de l'accélération **/
		if (erreurAlpha <= aFreinage && erreurAlpha >= -aFreinage)
			consigneAccelAlpha = -vitesseAlpha;						// Doit s'arreter (vitesseAlpha -> 0)
		else if (erreurAlpha < 0)
			consigneAccelAlpha = -vitesseMaxAlpha - vitesseAlpha;	// Doit reculer (vitesseAlpha -> -vitesseMaxAlpha)
		else consigneAccelAlpha = vitesseMaxAlpha - vitesseAlpha;	// Doit avancer (vitesseAlpha -> vitesseMaxAlpha)
		
		/** Limitation de l'accélération **/
		if (tempo_accel) consigneAccelAlpha = 0;
		else if (consigneAccelAlpha > 1) consigneAccelAlpha = 1;
		else if (consigneAccelAlpha < -1) consigneAccelAlpha = -1;
		tempo_accel ++;
		tempo_accel %= ACCEL_MAX_ROUE;
		//if (!cpt_boucle) send_message(consigneAccelAlpha);
	
		/** PI(D) Alpha **/
		erreurAlphaInterne += consigneVitesseAlpha - (vitesseAlpha << 5);	// [512ème de tck]
		if (cpt_boucle == 0) send_message(erreurAlphaInterne);
		integralErreurAlpha += erreurAlphaInterne;
		consigneVitesseAlpha += consigneAccelAlpha;							// [512ème de tck / période]
		//if (cpt_boucle == 10) send_message(consigneVitesseAlpha);
		commandeAlpha = ALPHA_CONS * consigneVitesseAlpha + ((ALPHA_P * erreurAlphaInterne) >> 9) + ((ALPHA_I * integralErreurAlpha) >> 15);
		//if (cpt_boucle == 10) send_message(commandeAlpha);
	
		/** PI(D) Delta **/
		integralErreurDelta += erreurDelta;
		commandeDelta = (long)((long long)commandeAlpha * rapportVitesse >> 15) + DELTA_P * erreurDelta + ((DELTA_I * integralErreurDelta) >> 6);
		//if (cpt_boucle == 0) send_message(commandeDelta);
	    
		/** Changement de variable (commandeDelta, commandeAlpha) -> (commandeR, commandeL) **/
	    commandeR = commandeDelta + commandeAlpha;
	    commandeL = commandeDelta - commandeAlpha;
	
		/** Evite le bloquage des roues /
		if(commandeR && commandeR > -10 && commandeR < 70) {	// Arrêt des moteurs à +53 et -0
			if (commandeR > 0) commandeR = 70;
			else commandeR = -10;
		}
		if(commandeL && commandeL > -10 && commandeL < 70) {	// Arrêt des moteurs à +53 et -0
			if (commandeL > 0) commandeL = 70;
			else commandeL = -10;
		} //*/
		//if (cpt_boucle == 0) send_message(commandeR);
		//if (cpt_boucle == 5) send_message(commandeL);
	
		/** Détection des débordements sur le moteur gauche **/
		//if (cpt_boucle == 10 && (commandeL < -MAX_SPEED || commandeL > MAX_SPEED)) send_message(0x11111111);
		//if (cpt_boucle == 10 && (commandeL < -MAX_SPEED || commandeL > MAX_SPEED)) send_message(erreurDelta);
		/** Détection des débordements sur le moteur droit **/
		//if (cpt_boucle == 20 && (commandeR < -MAX_SPEED || commandeR > MAX_SPEED)) send_message(0x22222222);
		//if (cpt_boucle == 10 && (commandeR < -MAX_SPEED || commandeR > MAX_SPEED)) send_message(erreurDelta);
	
	    /** Envoi des commandes aux moteurs **/
	    motors_set_speed(commandeL, commandeR);
		
		//*
		cpt_boucle ++;
		cpt_boucle %= 10; // Règle la fréquence d'envoie */
		if (!doitAttendre) _RB5 = 0;	// Timer trop rapide : allume LED debug
	}
}

void __attribute__((interrupt, auto_psv)) _MPWM1Interrupt(void) {
	IFS3bits.PWM1IF = 0;	// On s'acquitte de l'interruption
	doitAttendre = FALSE;
}

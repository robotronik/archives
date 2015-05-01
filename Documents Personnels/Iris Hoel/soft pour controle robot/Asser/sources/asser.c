/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/


//#define deriveErreurDelta_MAX 20000 * N * PERIODE / 1000 / D					// = 2*1m*N/D/(1000/PER)
#define VITESSE_MAX_LIN 3000														// Vitesse max linéaire en entrée du PWM -> donne moins de 0.5 m/s
																				// = VITESSE_MAX_LIN_ACTUEL * deriveErreurDelta_MAX / deriveErreurDelta_MAX_ACTUEL
//#define ACCEL_MAX_LIN (long)VITESSE_MAX_LIN * PERIOD / 500						// Augmentation maximale donnée au PID (Accélération max)
																				// '-> On atteint VITESSE_MAX_LIN en au moins 500/PERIOD boucles (500 ms)
#define ACCEL_MAX_LIN (int)5 / PERIOD	// /!\ Fct inverse

//#define NUM_DELTA_P (long)VITESSE_MAX_LIN/10	// Les divisions par des constantes "long" ne fonctionnent pas ; DELTA_P -> Fraction
//#define DENUM_DELTA_P (long)ERREUR_MAX_LIN/10
#define ALPHA_P 320	// >> 2 dans asser.c
#define ALPHA_I 1	// >> 8 dans asser.c
#define DELTA_P ALPHA_P	// >> 2 dans asser.c
#define DELTA_I ALPHA_I	// >> 8 dans asser.c

//#define COEF_FREIN	(int)((long)200 * N * PERIOD * PERIOD / 25 / D)	// = 417
#define COEF_FREIN	(int)1000

//void __attribute__((interrupt, auto_psv)) _T1Interrupt(void);


#include "main.h"

long consigneAlpha, consigneDelta; 	// Consigne (alpha, delta)
int rapportVitesse;					// Vitesse angulaire/vitesse linéaire
T_dividedLong distL, distR;			// Position de chaque roue pour un trajet
int doitAttendre = TRUE;
long erreurDelta = 0, erreurAlpha = 0;

void asser_delta() {
	long deltaMes, dFreinage,
		 alphaMes;	// Sens trigonométrique en vue de dessus
	static long long integralErreurAlpha = 0;
	short deriveErreurDelta;
	static short commandeDelta = 0;
	short deriveCommandeDelta, commandeAlpha;
	short commandeL, commandeR;
	static int tempo_accel = 0;

	static int cpt_boucle = 0;

    /** Actualisation position actuelle (distL, r) **/
    distL.part.low = motors_get_qei(LEFT);	//recupere la distance totale parcourue par une roue
    distR.part.low = motors_get_qei(RIGHT);	// X.part.high est mis à jour par interruption

    /** Changement variable (distL, r) -> (deltaMes, alphaMes) **/
    deltaMes = distR.all + distL.all;
    alphaMes = distR.all - distL.all;

	deriveErreurDelta = (consigneDelta - deltaMes) - erreurDelta;	// erreurDelta a la valeur précédente
	// (!cpt_boucle) send_message(deriveErreurDelta);
	erreurDelta += deriveErreurDelta;
	//if (!cpt_boucle) send_message(erreurDelta);
	//dFreinage = (long) deriveErreurDelta * deriveErreurDelta / COEF_FREIN;	// dFreinage = v² / COEF_FREIN
	dFreinage = (long) deriveErreurDelta * deriveErreurDelta * COEF_FREIN;
	//if (!cpt_boucle) send_message(dFreinage);

	/** Choix de la vitesse à atteindre (commande) et calcul de l'accélération **/
	if (erreurDelta <= dFreinage && erreurDelta >= -dFreinage)
		deriveCommandeDelta = -commandeDelta;
	else if (erreurDelta < 0) deriveCommandeDelta = -VITESSE_MAX_LIN - commandeDelta;
	else deriveCommandeDelta = VITESSE_MAX_LIN - commandeDelta;
	
	/** Limitation de l'accélération **/
	if (tempo_accel) deriveCommandeDelta = 0;
	else if (deriveCommandeDelta > 1) deriveCommandeDelta = 1;
	else if (deriveCommandeDelta < -1) deriveCommandeDelta = -1;
	commandeDelta += deriveCommandeDelta;
	//send_message(commandeDelta);
	tempo_accel ++;
	tempo_accel %= ACCEL_MAX_LIN;

	/** Evite de s'arrêter avec une erreur non-nulle **/
	if(erreurDelta && commandeDelta > -10 && commandeDelta < 70) {	// Arrêt à +53 et -0
		if (erreurDelta > 0) commandeDelta = 70;
		else commandeDelta = -10;
	}
	//if (!cpt_boucle) send_message(commandeDelta);

	/** PI(D) Alpha **/
	//deriveErreurAlpha = (deltaMes * rapportVitesse - alphaMes) - erreurAlpha;
	//erreurAlpha += deriveErreurAlpha;												// Calcul avec deriveErreurAlpha
	erreurAlpha = (long)((long long)deltaMes * rapportVitesse >> 16) - alphaMes;	// Calcul sans deriveErreurAlpha
	//if (!cpt_boucle) send_message(erreurAlpha);
	integralErreurAlpha += erreurAlpha;
	commandeAlpha = ((ALPHA_P * erreurAlpha) >> 2) + ((ALPHA_I * integralErreurAlpha) >> 8);
	//if (cpt_boucle == 5) send_message(commandeAlpha);
    
	/** Changement de variable (commandeDelta, commandeAlpha) -> (commandeR, commandeL) **/
    commandeR = ( commandeDelta + commandeAlpha );
    commandeL = ( commandeDelta - commandeAlpha );

	/** Détection des débordements sur le moteur gauche **/
	if (commandeL < -MAX_SPEED || commandeL > MAX_SPEED) send_message(0x11111111);
	//if (commandeL < -MAX_SPEED || commandeL > MAX_SPEED) send_message(commandeDelta);
	/** Détection des débordements sur le moteur droit **/
	if (commandeR < -MAX_SPEED || commandeR > MAX_SPEED) send_message(0x22222222);
	//if (commandeR < -MAX_SPEED || commandeR > MAX_SPEED) send_message(commandeDelta);

    /** Envoi des commandes aux moteurs **/
    motors_set_speed(RIGHT, commandeR);
    motors_set_speed(LEFT, commandeL);

	/*if(!(erreurDelta || deriveErreurDelta*//* || integralErreurDelta*//*)) {
		//_RB5 = 0;
		while (1) {
			_RB5 = 0;
			pause_s(1);
			_RB5 = 1;
			pause_s(1);
		}
		//send_message(DONE);
	}*/
	/*if(!deriveErreurDelta && erreurDelta) {
		send_message(commandeDelta);
	}*/
	cpt_boucle ++;
	cpt_boucle %= 200;
}

void asser_alpha() {
	long deltaMes, angleFreinage,
		 alphaMes;	// Sens trigonométrique en vue de dessus
	static long long integralErreurDelta = 0;
	short deriveErreurAlpha;
	static short commandeAlpha = 0;
	short deriveCommandeAlpha, commandeDelta;
	short commandeL, commandeR;
	static int tempo_accel = 0;

	static int cpt_boucle = 0;

    /** Actualisation position actuelle (distL, r) **/
    distL.part.low = motors_get_qei(LEFT);	//recupere la distance totale parcourue par une roue
    distR.part.low = motors_get_qei(RIGHT);	// X.part.high est mis à jour par interruption

    /** Changement variable (distL, r) -> (deltaMes, alphaMes) **/
    deltaMes = distR.all + distL.all;
	if (!cpt_boucle) send_message(deltaMes);
    alphaMes = distR.all - distL.all;

	deriveErreurAlpha = (consigneAlpha - alphaMes) - erreurAlpha;	// erreurAlpha a la valeur précédente
	//if (!cpt_boucle) send_message(deriveErreurAlpha);
	erreurAlpha += deriveErreurAlpha;
	//if (!cpt_boucle) send_message(erreurAlpha);
	//angleFreinage = (long) deriveErreurAlpha * deriveErreurAlpha / COEF_FREIN;	// dFreinage = v² / COEF_FREIN
	angleFreinage = (long) deriveErreurAlpha * deriveErreurAlpha * COEF_FREIN;
	//if (!cpt_boucle) send_message(angleFreinage);

	/** Choix de la vitesse à atteindre (commande) et calcul de l'accélération **/
	if (erreurAlpha <= angleFreinage && erreurAlpha >= -angleFreinage)
		deriveCommandeAlpha = -commandeAlpha;
	else if (erreurAlpha < 0) deriveCommandeAlpha = -VITESSE_MAX_LIN - commandeAlpha;
	else deriveCommandeAlpha = VITESSE_MAX_LIN - commandeAlpha;
	
	/** Limitation de l'accélération **/
	if (tempo_accel) deriveCommandeAlpha = 0;
	else if (deriveCommandeAlpha > 1) deriveCommandeAlpha = 1;
	else if (deriveCommandeAlpha < -1) deriveCommandeAlpha = -1;
	commandeAlpha += deriveCommandeAlpha;
	//if (!cpt_boucle) send_message(commandeAlpha);
	tempo_accel ++;
	tempo_accel %= ACCEL_MAX_LIN;

	/** Evite de s'arrêter avec une erreur non-nulle **/
	if(erreurAlpha && commandeAlpha > -70 && commandeAlpha < 70) {	// Arrêt à +53 et -0
		if (erreurAlpha > 0) commandeAlpha = 70;
		else commandeAlpha = -70;
	}
	//if (!cpt_boucle) send_message(commandeAlpha);

	/** PI(D) Alpha **/
	//deriveErreurAlpha = (alphMes * rapportVitesse - deltaMes) - erreurDelta;
	//erreurDelta += deriveErreurDelta;												// Calcul avec deriveErreurDelta
	erreurDelta = (long)((long long)alphaMes * rapportVitesse >> 16) - deltaMes;	// Calcul sans deriveErreurDelta
	//if (!cpt_boucle) send_message(erreurDelta);
	integralErreurDelta += erreurDelta;
	commandeDelta = ((DELTA_P * erreurDelta) >> 2) + ((DELTA_I * integralErreurDelta) >> 8);
	//if (cpt_boucle == 5) send_message(commandeDelta);
    
	/** Changement de variable (commandeDelta, commandeAlpha) -> (commandeR, commandeL) **/
    commandeR = ( commandeDelta + commandeAlpha );
    commandeL = ( commandeDelta - commandeAlpha );

	/** Détection des débordements sur le moteur gauche **/
	if (commandeL < -MAX_SPEED || commandeL > MAX_SPEED) send_message(0x11111111);
	//if (commandeL < -MAX_SPEED || commandeL > MAX_SPEED) send_message(commandeDelta);
	/** Détection des débordements sur le moteur droit **/
	if (commandeR < -MAX_SPEED || commandeR > MAX_SPEED) send_message(0x22222222);
	//if (commandeR < -MAX_SPEED || commandeR > MAX_SPEED) send_message(commandeDelta);

    /** Envoi des commandes aux moteurs **/
    motors_set_speed(RIGHT, commandeR);
    motors_set_speed(LEFT, commandeL);
    
	cpt_boucle ++;
	cpt_boucle %= 200;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
	IFS0bits.T1IF = 0; // On s'acquitte de l'interruption
	doitAttendre = FALSE;
}

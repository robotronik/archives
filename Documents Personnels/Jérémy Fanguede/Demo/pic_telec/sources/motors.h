/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/
 
#ifndef _MOTORS_H
#define _MOTORS_H

#define MAX_SPEED 3999	// Entrée max des PWMs = 2*Période des PWMs
						// Fréquence des PWMs = 20 kHz
#define MIN_SPEED 600	// Entrée min sauf pour l'arrêt
#define ASSER_PERIOD 5	// [demi-dixième de ms] postscaler des interruptions du PWM1
						// 5 -> 4 kHz (1 tck max par codeur par période d'asser)

//#define ACCEL_MAX_ROUE_INT	(short)((long)200000 * D / N / ASSER_PERIOD / ASSER_PERIOD)
//#define ACCEL_MAX_ROUE		1//(ACCEL_MAX_ROUE_INT >> 9)	// [periode²/512ème de tck] Temps entre chaque accélération de 1 = 1/(0,1 m/s²) (= 7)
#define ACCEL_MAX_ROUE2		1

/* motors_set_speed : règle la vitesse d'un moteur, ie le rapport cyclique du PWM associé */
void motors_set_speed (short speedL, short speedR);
void motors_stop(void); // Arrete les moteurs

/* motors_get_qei : renvoie la lecture actuelle d'un codeur incrémentiel */
void motors_get_qei(long *pPosL, long *pPosR);

/* motors_reset_qei : réinitialise le codeur incrémental */
void motors_reset_qei();

#define commandeMoteur																	\
	static short consR = 0, consL = 0; /* Commande des moteurs avant l'anti-bloquage */	\
																						\
	/** (delta, alpha) -> (R, L) **/													\
	/** Limitation de l'accélération **/												\
	short commandeAlphaPrec = consR - consL, commandeDeltaPrec = consR + consL;			\
	if (consMAlpha >= commandeAlphaPrec) {												\
		if (consMDelta >= commandeDeltaPrec)												\
			consR += ACCEL_MAX_ROUE2;													\
		if (consMDelta <= commandeDeltaPrec)												\
			consL -= ACCEL_MAX_ROUE2;													\
	} if (consMAlpha <= commandeAlphaPrec) {											\
		if (consMDelta >= commandeDeltaPrec)												\
			consL += ACCEL_MAX_ROUE2;													\
		if (consMDelta <= commandeDeltaPrec)												\
			consR -= ACCEL_MAX_ROUE2;													\
	}																					\
																						\
	/** Evite le bloquage des roues */													\
	short commandeL, commandeR;															\
	if(consL && consL > -min_speed && consL < min_speed) {								\
		if (consL > 0) commandeL = min_speed;											\
		else commandeL = -min_speed;													\
	} else commandeL = consL;															\
	if(consR && consR > -min_speed && consR < min_speed) {								\
		if (consR > 0) commandeR = min_speed;											\
		else commandeR = -min_speed;													\
	} else commandeR = consR; /**/														\
																						\
	/** Envoi des commandes aux moteurs **/												\
	motors_set_speed(commandeL, commandeR);
#endif

/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/

#include "main.h"
#include "motors.h"
#include "QEI.h"
#include "asser.h"

#if defined(COLLECTEUR)
#include "../Collecteur/src/AsserGlue.h"
#elif defined(SIMU)	// #if defined(COLLECTEUR) || defined(SIMU)
#include "../SimuAsserPSoC/src/AsserGlue.h"
#endif	// #if defined(COLLECTEUR) || defined(SIMU)

//#define NOP

/* Réglages */
#define NB_ECH_VIT				8		// [per]
#define NB_ECH_DETECT_FREIN		256		// [per]
#define NIV_BAT_INV_INIT		100		// Max vu sur nivBatInv

/* Ne pas modifier */
/*#define MAX_ALLOCABLE			512
#if NB_ECH_DIST_VIT > MAX_ALLOCABLE
#define NB_ECH_DIST_VIT2		MAX_ALLOCABLE
#else	// #if TEMPS_MOYS > MAX_ALLOCABLE
#define NB_ECH_DIST_VIT2		NB_ECH_DIST_VIT
#endif	// #if TEMPS_MOYS > MAX_ALLOCABLE
#define NB_ECHANT_BAT			(NB_ECH_DIST_VIT / NB_ECH_DIST_VIT2)*/

unsigned char	accel_max = ACCEL_MAX_INIT;
unsigned char	accel_max_int = ACCEL_MAX_INIT;
unsigned char	deccel_max = DECCEL_MAX;
short consMDelta, consMAlpha;
short commandeDeltaPrec = 0, commandeAlphaPrec = 0;
short commandeL = 0, commandeR = 0, commandeLHaut = 0;
unsigned short minSpeedL = 0, minSpeedR = 0, minSpeed = 0;

// Pour calcul de l'accélération max
//unsigned char cptBat = 0;
long distLPrec[NB_ECH_VIT] = {0};			// [tck]
short vitLMoy = 0;
short vitLMoyPrec[NB_ECH_DETECT_FREIN] = {0};
short vitLMoyHaut = 0;
short cmdLPrec[NB_ECH_DETECT_FREIN] = {0};
//short cmdRPrec[TAILLE_TAB_VIT_ROUE] = {0};
boolean cmdFrein = FALSE, vitFrein = FALSE;
unsigned short indexTabVit = 0, indexTabDetectFrein = 0;
short nivBatInv = NIV_BAT_INV_INIT;

inline void motorsSetAlphaDeltaSpeed(void) {
	/* static pour mesure mémoire */
	static short consMDelta2, consMAlpha2;
	static short comDeltaLimit = 0, comAlphaLimit = 0;
	
	/* static normal */
//	static boolean isLMoving = FALSE, isRMoving = FALSE;
	
	consMDelta2 = consMDelta << 2;
	consMAlpha2 = consMAlpha << 2;
	if (comDeltaLimit > 0) {
		if      (comDeltaLimit + accel_max_int <= consMDelta2) comDeltaLimit += accel_max_int;
		else if	(comDeltaLimit - deccel_max    >= consMDelta2) comDeltaLimit -= deccel_max;
		else                                                   comDeltaLimit = consMDelta2;
	} else {
		if      (comDeltaLimit + deccel_max    <= consMDelta2) comDeltaLimit += deccel_max;
		else if	(comDeltaLimit - accel_max_int >= consMDelta2) comDeltaLimit -= accel_max_int;
		else                                                   comDeltaLimit = consMDelta2;
	}
	if (comAlphaLimit > 0) {
		if		(comAlphaLimit + accel_max_int <= consMAlpha2) comAlphaLimit += accel_max_int;
		else if	(comAlphaLimit - deccel_max    >= consMAlpha2) comAlphaLimit -= deccel_max;
		else                                                   comAlphaLimit = consMAlpha2;
	} else {
		if		(comAlphaLimit + deccel_max    <= consMAlpha2) comAlphaLimit += deccel_max;
		else if	(comAlphaLimit - accel_max_int >= consMAlpha2) comAlphaLimit -= accel_max_int;
		else                                                   comAlphaLimit = consMAlpha2;
	}
	commandeAlphaPrec = comAlphaLimit >> 2;
	commandeDeltaPrec = comDeltaLimit >> 2;
	
	/*	// minSpeedL
	if (tabCmdLRetardPrec[indexTabRoue] != 0 && !isLMoving && (vitL > 2 || vitL < -2)) {
		isLMoving = TRUE;
		if (tabCmdLRetardPrec[indexTabRoue] > 0)
			minSpeedL = tabCmdLRetardPrec[indexTabRoue];
		else
			minSpeedL = -tabCmdLRetardPrec[indexTabRoue];
	} else if (!vitL) isLMoving = FALSE;
		// minSpeedR
	if (tabCmdRRetardPrec[indexTabRoue] != 0 && !isRMoving && (vitR > 2 || vitR < -2)) {
		isRMoving = TRUE;
		if (tabCmdRRetardPrec[indexTabRoue] > 0)
			minSpeedR = tabCmdRRetardPrec[indexTabRoue];
		else
			minSpeedR = -tabCmdRRetardPrec[indexTabRoue];
	} else if (!vitR) isRMoving = FALSE;
	if (minSpeedR < minSpeedL) minSpeed = minSpeedR;
	else minSpeed = minSpeedL;*/
	
	/** (delta, alpha) -> (R, L) **/
	commandeR = commandeDeltaPrec + commandeAlphaPrec;
	commandeL = commandeDeltaPrec - commandeAlphaPrec;
	
	/** Evite le bloquage des moteurs à faible vitesse */
	if(commandeL && commandeL > -minSpeedL && commandeL < minSpeedL) {
		if (commandeL > 0) commandeL = minSpeedL;
		else commandeL = -minSpeedL;
	}
	if(commandeR && commandeR > -minSpeedR && commandeR < minSpeedR) {
		if (commandeR > 0) commandeR = minSpeedR;
		else commandeR = -minSpeedR;
	}
	
	/** Envoi des commandes aux moteurs **/
	motors_set_speed(commandeL, commandeR);
}	

inline void commandeMoteur(boolean simple) {
	if (!simple) {
		/** Limitation de l'accélération **/
		if (commandeL > 0) commandeL -= minSpeedL;
		else if (commandeL < 0) commandeL += minSpeedL;
		if (commandeR > 0) commandeR -= minSpeedR;
		else if (commandeR < 0) commandeR += minSpeedR;
		//if (!cptBat) {
				// Commande avant dernier freinage
			if (cmdLPrec[indexTabDetectFrein] > 0) {
				if (cmdFrein ^ (cmdLPrec[indexTabDetectFrein] - commandeL > (deccel_max*NB_ECH_DETECT_FREIN >> 4))) {
					if (!cmdFrein) commandeLHaut = cmdLPrec[indexTabDetectFrein];
					cmdFrein = !cmdFrein;
				}
			} else {
				if (cmdFrein ^ (cmdLPrec[indexTabDetectFrein] - commandeL < -(deccel_max*NB_ECH_DETECT_FREIN >> 4))) {
					if (!cmdFrein) commandeLHaut = -cmdLPrec[indexTabDetectFrein];
					cmdFrein = !cmdFrein;
				}
			}
				// Moyenne de vitesse mesurée
			vitLMoy = distL - distLPrec[indexTabVit];
				// Vitesse avant dernier freinage
			if (vitLMoyPrec[indexTabDetectFrein] > 0) {
				if (vitFrein ^ (vitLMoyPrec[indexTabDetectFrein] - vitLMoy > (NB_ECH_VIT*NB_ECH_DETECT_FREIN >> 10))) {
		    			// Niveau de batterie approché, non calculée à vitesse faible
					if (!vitFrein && commandeLHaut > 3000) {
						vitLMoyHaut = vitLMoyPrec[indexTabDetectFrein];
						nivBatInv = commandeLHaut / vitLMoyHaut;
						commandeLHaut = 0;
							// Accélération
						accel_max_int = (unsigned short) mulUU(accel_max, nivBatInv) / (unsigned short) NIV_BAT_INV_INIT;
					}
					vitFrein = !vitFrein;
				}
			} else {
				if (vitFrein ^ (vitLMoyPrec[indexTabDetectFrein] - vitLMoy < -(NB_ECH_VIT*NB_ECH_DETECT_FREIN >> 10))) {
		    			// Niveau de batterie approché, non calculée à vitesse faible
					if (!vitFrein && commandeLHaut > 3000) {
						vitLMoyHaut = -vitLMoyPrec[indexTabDetectFrein];
						nivBatInv = commandeLHaut / vitLMoyHaut;
						commandeLHaut = 0;
							// Accélération
						accel_max_int = (unsigned short) mulUU(accel_max, nivBatInv) / (unsigned short) NIV_BAT_INV_INIT;
					}
					vitFrein = !vitFrein;
				}
			}
				// Gestion des tableaux
			distLPrec[indexTabVit] = distL;
			vitLMoyPrec[indexTabDetectFrein] = vitLMoy;
			cmdLPrec[indexTabDetectFrein] = commandeL;
			//cmdRPrec[indexTabRoue] = commandeR;
			indexTabVit ++; indexTabVit %= NB_ECH_VIT;
			indexTabDetectFrein ++; indexTabDetectFrein %= NB_ECH_DETECT_FREIN;
				// Si dérapage...
			/*if (((vitLMoy*nivBatInv) < commandeLMoy && (commandeLMoy < -20*TAILLE_TAB_VIT_ROUE>>1 || commandeLMoy > 20*TAILLE_TAB_VIT_ROUE>>1)) 
				|| ((vitRMoy*nivBatInv) < commandeRMoy && (commandeRMoy < -20*TAILLE_TAB_VIT_ROUE>>1 || commandeRMoy > 20*TAILLE_TAB_VIT_ROUE>>1))) {
				accel_max --;
				consMDelta = 0;
				consMAlpha = 0;
			}*/
		/*}
		cptBat ++; cptBat %= NB_ECHANT_BAT;*/
	}	
	
	motorsSetAlphaDeltaSpeed();
}

inline void freine(void) {
	consMDelta = 0;
	consMAlpha = 0;
}

#if !defined(COLLECTEUR) && !defined(SIMU)
inline void motors_set_speed(short speedL, short speedR) {
#ifndef NOP
   	if (speedL == 0)    // On s arrete
    {
        PWM1CON1bits.PEN1H = 0;	// /!\ Reset le timer
        PWM1CON1bits.PEN1L = 0;
    }
    else if(speedL >= MAX_SPEED){
        PWM1CON1bits.PEN1H = 0;
        PWM1CON1bits.PEN1L = 1;
        P1DC1 = MAX_SPEED;
    }
    else if(speedL <= -MAX_SPEED){
        PWM1CON1bits.PEN1H = 1;
        PWM1CON1bits.PEN1L = 0;
        P1DC1 = MAX_SPEED;
    }
	else if (speedL > 0)  // On avance
    {
	    PWM1CON1bits.PEN1H = 0;
	    PWM1CON1bits.PEN1L = 1;
	    P1DC1 = speedL;
    }
   	else if (speedL < 0)   // On recule
    {
        PWM1CON1bits.PEN1H = 1;
        PWM1CON1bits.PEN1L = 0;
        P1DC1 = -speedL;
    }
    
   	if (speedR == 0)    // On s arrete
    {
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 0;
    }
    else if(speedR >= MAX_SPEED){
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 1;
        P1DC2 = MAX_SPEED;
    }
    else if(speedR <= -MAX_SPEED){
        PWM1CON1bits.PEN2H = 1;
        PWM1CON1bits.PEN2L = 0;
        P1DC2 = MAX_SPEED;
    }
	else if (speedR > 0)  // On avance
    {
        PWM1CON1bits.PEN2H = 0;
        PWM1CON1bits.PEN2L = 1;
        P1DC2 = speedR;
    }
   	else if (speedR < 0)   // On recule
    {
        PWM1CON1bits.PEN2H = 1;
        PWM1CON1bits.PEN2L = 0;
        P1DC2 = -speedR;
    }
#endif	// #ifndef NOP
}

inline void motors_stop(void)
{
	// On met les 4 sorties en logique
	PWM1CON1bits.PEN1H = 0;
    PWM1CON1bits.PEN2H = 0;
    PWM1CON1bits.PEN1L = 0;
    PWM1CON1bits.PEN2L = 0;
}
#endif	// #if !defined(COLLECTEUR) && !defined(SIMU)

inline void reglageCmdMin1(void) {
	accel_max_int = 1;
	minSpeedL = minSpeedR = 0;
	
	consMDelta = 0;
	consMAlpha = MAX_SPEED;
	
	commandeCourante ++;
}

inline void reglageCmdMin2(void) {
	if (vitL && !minSpeedL) minSpeedL = -commandeL;
	if (vitR && !minSpeedR) minSpeedR = commandeR;
	if (minSpeedL && minSpeedR)
		commandeCourante ++;
}

inline void reglageCmdMin3(void) {
	freine();
	if (!(commandeL || commandeR))
		chargeCommandeSuiv();
}

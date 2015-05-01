/*-----------------------------------------------------*
 * Gestion des moteurs -> utilisation des pwm          *
 *-----------------------------------------------------*/
 
#ifndef _MOTORS_H
#define _MOTORS_H

#define PWM_PERIOD 1999	// 40 MHz / PWM_PERIOD = Fr�quence des PWMs
#define MAX_SPEED 3999	// Entr�e max des PWMs (< ou = 2*PWM_PERIOD)
#define ASSER_PERIOD 5	// [PWM_PERIOD] postscaler des interruptions du PWM1 :
						// 40 MHz / PWM_PERIOD / ASSER_PERIOD = Fr�quence de la boucle d'asser
#define ASSER_FREQ (40*1000*1000/PWM_PERIOD/ASSER_PERIOD)

#define ACCEL_MAX_INIT	1	// [SPEED / 4periodes] Acc�l�ration (4 <=> 1 m/s�)
#define DECCEL_MAX		2

extern unsigned char accel_max;
extern unsigned char accel_max_int;
extern unsigned char deccel_max;
extern short vitLMoy;
extern short vitLMoyHaut;
extern short nivBatInv;
extern short consMDelta, consMAlpha;
extern short commandeDeltaPrec, commandeAlphaPrec;
extern short commandeL, commandeR, commandeLHaut;
extern unsigned short minSpeed;

inline void freine(void);
/* motors_set_speed : r�gle la vitesse d'un moteur, ie le rapport cyclique du PWM associ� */
inline void motors_set_speed (short speedL, short speedR);
inline void motors_stop(void); // Arrete les moteurs

/* anti-bloquage + limite d'acc�l�ration */
inline void commandeMoteur(boolean simple);
inline void reglageCmdMin1(void);
inline void reglageCmdMin2(void);
inline void reglageCmdMin3(void);

#endif

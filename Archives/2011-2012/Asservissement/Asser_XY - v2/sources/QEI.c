#include "main.h"

short distLHigh, distRHigh;	// Suppléments aux buffers matériels du QEI, à echantillonner
long distL = 0, distR = 0;	// Position absolue de chaque roue [tck]
char vitL, vitR;			// Vitesse absolue de chaque roue [tck/per]

#if !defined(COLLECTEUR) && !defined(SIMU)
inline void getQEI(long *pPosL, long *pPosR)
{
	((T_dividedLong *) pPosL)->part.high = distLHigh;
	((T_dividedLong *) pPosL)->part.low = POS1CNT;
	((T_dividedLong *) pPosR)->part.high = distRHigh;
	((T_dividedLong *) pPosR)->part.low = POS2CNT;
}

inline void resetQEI()
{
	POS1CNT = 0;
	distLHigh = 0;
	POS2CNT = 0;
	distRHigh = 0;
}

void __attribute__((interrupt, auto_psv)) _QEI1Interrupt(void)
{
	IFS3bits.QEI1IF = 0; // On s'acquitte de l'interruption
	if ((unsigned short) POS1CNT < 32768) distLHigh ++;
	else distLHigh --;
}

void __attribute__((interrupt, auto_psv)) _QEI2Interrupt(void)
{
	IFS4bits.QEI2IF = 0; // On s'acquitte de l'interruption
	if ((unsigned short) POS2CNT < 32768) distRHigh ++;
	else distRHigh --;
}
#endif	// #if !defined(COLLECTEUR) && !defined(SIMU)

#ifndef _QEI_H
#define _QEI_H

extern long distL, distR;				// Position absolue de chaque roue [tck]
extern char vitL, vitR;					// Vitesse absolue de chaque roue [tck/per]

/* renvoie la lecture actuelle d'un codeur incrémental */
inline void getQEI(long *pPosL, long *pPosR);

/* réinitialise le codeur incrémental */
inline void resetQEI();

#endif

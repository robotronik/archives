#ifndef _XYANGLE_H
#define _XYANGLE_H

// P  <-  R
//    X   |  alpha trigo, 0° à droite
//      Y v

#include "main.h"

extern T_dividedULong posX;
extern T_dividedULong posY;
extern unsigned short posAlpha;
extern unsigned short alphaInit;

inline void getXYAngle(void);
inline void getErreurs(void);
inline void purpleStart(void);
inline void redStart(void);
inline void sendPos(void);

#endif

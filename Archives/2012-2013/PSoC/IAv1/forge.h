/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef FORGE_H
#define FORGE_H

#define NB_PILES 5 //3*4=12
#define DIST_LIBER 170;

#include "constantes.h"

void initPiles(void);
void initMission(Mission * m);

inline int genererNop(Action *a, Pos pPrec);

void forgerMissionVerre(Mission *m, Pos pInit);
//void forgerMissionVerreNous(Mission * m, Pos pInit);
//void forgerMissionVerreAdv(Mission * m, Pos pInit);
void forgerMissionVerre4(Mission * m, Pos pInit);
void forgerMissionDeposerPile(Mission * m, Pos pInit);
void forgerMissionGateau(Mission * m, Pos pInit);
void forgerMissionBougiesFaciles(Mission * m, Pos pInit);
void forgerMissionCallageHaut(Mission * m, Pos pInit);
void forgerMissionCallageBuffet(Mission * m, Pos pInit);
void forgerMissionRatissage(Mission * m, Pos pInit);
void forgerMissionToutDroit(Mission * m, Pos pInit);

int forgerMissionCartoEvitementAstar(Mission *m, Pos pDep, Pos pObst);

void forgerMissionAuDepart(Mission * m, Pos pInit);
void forgerMissionCarre(Mission * m, Pos pInit);
void forgerMissionDemiTours(Mission * m, Pos pInit);

#endif
//[] END OF FILE

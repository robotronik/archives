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

#ifndef CARTOGRAPHIE_H_INCLUDED
#define CARTOGRAPHIE_H_INCLUDED

#include "constantes.h"

void initCarto(void);

int genererEvitementAstar(Action * tab, Pos pDep, Pos pArr, Pos pObst, int * status);
int pointDansDisque(Pos p, Pos centre, int rayon);

#endif // CARTOGRAPHIE_H_INCLUDED
//[] END OF FILE

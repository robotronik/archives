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
#include "constantes.h"
 int genererActionDelta( Action * a, int val, Pos pF);
 int genererActionAlphaAbs( Action * a, int alpha, Pos pF);
 int genererActionAlpha( Action * a, int alpha);
 int genererActionsXY(Action tab[2], int x, int y);
 int genererActionAlphaFrom(Action * a, int alphaOr, int alphaDst);
 int genererActionAlpha( Action * a, int alpha);
 int genererActionsXYFromPos(Action tab[2], Pos pOr, Pos pDst);
 int genererActionsXYFromPosAlphaFinal(Action tab[3], Pos pOr, Pos pDst);
 int genererActionsXY(Action tab[2], int x, int y);
 int genererActionsToXYPos(Action tab[3], Pos pDst);
 int genererActionsToXYAlphaFinal(Action tab[4], Pos pDst);
 int genererActionsXYFromPosLong(Action tab[5], Pos pOr, Pos pDst);
 int genererActionsToXYPosLong(Action tab[6], Pos pDst);
 
Pos findAJumpTo(Action * tab, int index, Pos pObst, int * newIndex);

//int moveToAlpha(short alpha);
//[] END OF FILE

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

#ifndef _INIT_H
#define _INIT_H

#include "Boolean.h"

typedef enum {ATTENTE, JEU, TEST_PHASE} phase_t;

extern boolean isRed;
extern phase_t gamePhase;

inline void PSoCinit();

#endif
//[] END OF FILE

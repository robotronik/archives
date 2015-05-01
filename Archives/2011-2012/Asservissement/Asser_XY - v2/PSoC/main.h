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
#ifndef MAIN_H
#define MAIN_H

#ifdef COLLECTEUR
	#include "../../Collecteur/src/PSoCGlue.h"
#elif defined(SIMU)	// #ifndef COLLECTEUR
	#include "../../SimuAsserPSoC/src/PSoCGlue.h"
#else
	#include <device.h>
#endif	// #ifndef COLLECTEUR
#include "Boolean.h"

#define SEND_COLOR	1	// config du mode test seulement
#define DO_CALLE	0

#define SERVO		0

void fin_IRQ_Handler();

#endif

//[] END OF FILE

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

#include "porte.h"
#include "constantes.h"
#include "PWM_ServoPorte.h"
#include "TimerResetCtrl.h"

extern int libererMAE;

void libererVerre(void)
{
	libererMAE = 1;
}

void ouvrirPorte(void)
{
	PWM_ServoPorte_WriteCompare(150);
	DEBUG_PRINTF("ouvrir porte");
}

void fermerPorte(void)
{
	PWM_ServoPorte_WriteCompare(85);//73
	DEBUG_PRINTF("fermer porte");
}

void toucherAvecPorte(void)
{
	PWM_ServoPorte_WriteCompare(105);
	DEBUG_PRINTF("toucher porte");
}

inline void resetTimerEmpileur(void)
{
	TimerResetCtrl_Write(1);
	CyDelay(TEMPS_RESET);
	TimerResetCtrl_Write(0);
}

/* [] END OF FILE */

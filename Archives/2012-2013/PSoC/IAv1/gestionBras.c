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
#include "gestionBras.h"
#include "PWM_ServoDroitHaut.h"
#include "PWM_ServoDroitBas.h"
#include "PWM_ServoGaucheHaut.h"
#include "PWM_ServoGaucheBas.h"
#include "TimerResetCtrlBras.h"
#include "Timer_Bras.h"


volatile int finTimerBrasN = 1;
extern Couleur couleur;

inline void resetTimerBras(void)
{
	TimerResetCtrlBras_Write(1);
	CyDelay(TEMPS_RESET);
	TimerResetCtrlBras_Write(0);
	finTimerBrasN = 1;
	Timer_Bras_Start();
}

void armerBrasDroitHaut(void)
{
	PWM_ServoDroitHaut_WriteCompare(POS_DROIT_HAUT_ARMER);
}
void replierBrasDroitHaut(void)
{
	PWM_ServoDroitHaut_WriteCompare(POS_DROIT_HAUT_REPLIER);
	resetTimerBras();
}
void taperBrasDroitHaut(void)
{
	PWM_ServoDroitHaut_WriteCompare(POS_DROIT_HAUT_TAPER);
	resetTimerBras();
}
void starterBrasDroitHaut(void)
{
	PWM_ServoDroitHaut_Start();
}
void stopBrasDroitHaut(void)
{
	PWM_ServoDroitHaut_Stop();
}


void armerBrasDroitBas(void)
{
	PWM_ServoDroitBas_WriteCompare(POS_DROIT_BAS_ARMER);
}
void replierBrasDroitBas(void)
{
	PWM_ServoDroitBas_WriteCompare(POS_DROIT_BAS_REPLIER);
	resetTimerBras();
}
void taperBrasDroitBas(void)
{
	PWM_ServoDroitBas_WriteCompare(POS_DROIT_BAS_TAPER);
	resetTimerBras();
}
void starterBrasDroitBas(void)
{
	PWM_ServoDroitBas_Start();
}
void stopBrasDroitBas(void)
{
	PWM_ServoDroitBas_Stop();
}



void armerBrasGaucheHaut(void)
{
	PWM_ServoGaucheHaut_WriteCompare(POS_GAUCHE_HAUT_ARMER);
}
void replierBrasGaucheHaut(void)
{
	PWM_ServoGaucheHaut_WriteCompare(POS_GAUCHE_HAUT_REPLIER);
	resetTimerBras();
}
void taperBrasGaucheHaut(void)
{
	PWM_ServoGaucheHaut_WriteCompare(POS_GAUCHE_HAUT_TAPER);
	resetTimerBras();
}
void starterBrasGaucheHaut(void)
{
	PWM_ServoGaucheHaut_Start();
}
void stopBrasGaucheHaut(void)
{
	PWM_ServoGaucheHaut_Stop();
}



void armerBrasGaucheBas(void)
{
	PWM_ServoGaucheBas_WriteCompare(POS_GAUCHE_BAS_ARMER);
}
void replierBrasGaucheBas(void)
{
	PWM_ServoGaucheBas_WriteCompare(POS_GAUCHE_BAS_REPLIER);
	resetTimerBras();
}
void taperBrasGaucheBas(void)
{
	PWM_ServoGaucheBas_WriteCompare(POS_GAUCHE_BAS_TAPER);
	resetTimerBras();
}
void starterBrasGaucheBas(void)
{
	PWM_ServoGaucheBas_Start();
}
void stopBrasGaucheBas(void)
{
	PWM_ServoGaucheBas_Stop();
}


void armerBrasHaut(void)
{
	if (couleur == BLEU)
		armerBrasDroitHaut();
	else
		armerBrasGaucheHaut();
}
void replierBrasHaut(void)
{
	if (couleur == BLEU)
		replierBrasDroitHaut();
	else
		replierBrasGaucheHaut();
}
void taperBrasHaut(void)
{
	if (couleur == BLEU)
		taperBrasDroitHaut();
	else
		taperBrasGaucheHaut();
}
void starterBrasHaut(void)
{
	if (couleur == BLEU)
		starterBrasDroitHaut();
	else
		starterBrasGaucheHaut();
}
void stopBrasHaut(void)
{
	if (couleur == BLEU)
		stopBrasDroitHaut();
	else
		stopBrasGaucheHaut();
}




void armerBrasBas(void)
{
	if (couleur == BLEU)
		armerBrasDroitBas();
	else
		armerBrasGaucheBas();
}
void replierBrasBas(void)
{
	if (couleur == BLEU)
		replierBrasDroitBas();
	else
		replierBrasGaucheBas();
}
void taperBrasBas(void)
{
	if (couleur == BLEU)
		taperBrasDroitBas();
	else
		taperBrasGaucheBas();
}
void starterBrasBas(void)
{
	if (couleur == BLEU)
		starterBrasDroitBas();
	else
		starterBrasGaucheBas();
}
void stopBrasBas(void)
{
	if (couleur == BLEU)
		stopBrasDroitBas();
	else
		stopBrasGaucheBas();
}

/* [] END OF FILE */

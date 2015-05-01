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

#include "global.h"
#include "init.h"
#include "strategie.h"
#include "servo.h"
#include <device.h>

extern Couleur g_couleur;

void init()
{
	// Initialiser des communications
    UART_XBee_Start();
	UART_XBee_RX_isr_Start();
	UART_Asser_Start();
	UART_Asser_RX_isr_Start();
	
	// Initialisation des actionneurs
	isr_Fin_Start();
	//MuxSharp_Select(1);
	//ADCSharp_Start();
	//ADCSharp_StartConvert();
	//PWM_ServoBrasDroit_Start();
    //PWM_ServoBrasGauche_Start();
    //PWM_ServoQueue_Start();
	/*BumperGaucheISR_Start();
	BumperDroitISR_Start();
	BumperGaucheFinISR_Start();
	BumperDroitFinISR_Start();*/
    //  initPosServo();
	
	if(Couleur_Read() == 1)
	{
		g_couleur = JAUNE;
	}
	else
	{
		g_couleur = ROUGE;
	}

    // Definir les objectifs
    strategie();
    
    // Initialiser la position (en fct de la couleur)
    // Robots : 300*340 mm
    
}

/* [] END OF FILE */

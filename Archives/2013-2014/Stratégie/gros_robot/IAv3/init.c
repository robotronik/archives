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

#include <device.h>
#include "init.h"
#include "comAsser.h"
#include "global.h"
#include "strategie.h"

extern Couleur g_couleur;
extern Etat etatCourant;

void initPhase1(void)
{
    UART_XBee_Start();
    ISR_RX_XBee_Start();
    UART_Asser_Start();
    ISR_RX_Asser_Start();
    
    //Timer_Fin_Start();//plutot à mettre lors de la tirette
    //ISR_Fin_Start();
    
//    MuxSharp_Select(0);
    
    ADC_Sharp_1_Start();
    ADC_Sharp_1_StartConvert();
    ADC_Sharp_2_Start();
    ADC_Sharp_2_StartConvert();
    //Comp_Sharp_1_Start();
    //Comp_Sharp_2_Start();
    
    PWM_LanceurDroit_Start();
    PWM_LanceurGauche_Start();
    PWM_Queue_Start();
    
    /*ISR_BumperGauche_Start();
    ISR_BumperGauche_Fin_Start();
    ISR_BumperDroit_Start();
    ISR_BumperDroit_Fin_Start();*/
    
 }
 
 void initPhase2(void)
 {
    //lecture couleur
    if(Couleur_Read() == 0)
	{
		g_couleur = JAUNE;
	}
	else
	{
		g_couleur = ROUGE;
	}
    
    // Definir les objectifs
    strategie();
    
    //envoyer les positions
    if(JAUNE == g_couleur)
    {
        sendSetAlpha(0);
        CyDelay(100);
        sendSetXY(150, 2770);
    }
    else
    {
        sendSetAlpha(0);
        CyDelay(100);
        sendSetXY(150, 230);
    }
    
    etatCourant = ATTENTE_TIRETTE;
 }
/* [] END OF FILE */

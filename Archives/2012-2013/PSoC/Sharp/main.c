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

/*Fonctions : int conversion(int entree); => donne la distance en fonction de la sortie analogique du sharp
				ATTENTION : Distance approximative, dépend beaucoup de la couleur observée, de l'angle d'observation
							Après 1000 mm, plutôt considérer "présence/non présence" au lieu de distance
		
				void initTableau(Calibre *calibre);=> initialise le tableau avec les valeur du calibrage sur BLANC lumière modérée et angle 0 (parallèle)
				idée : lecture de fichier pour changer plus facilement le calibre
*/
		
#include "device.h"
#include "servo.h"

void main()
{
    int x=0;
    CyGlobalIntEnable;
	ADCSharp_Start();
	PWMServoSharp_Start();
	TimerServoSharp_Start();
	intSharp_Start();
	MuxSharp_Start();
    
    
	while(1)
	{
        if(bouton_Read()) x=1;
        else x=0;
        
        if(detectionSharp(x+1))
        {
        LED1_Write(0);
        LED2_Write(0);
        LED3_Write(0);
        LED4_Write(0);
        LED5_Write(0);
        LED6_Write(0);
        LED7_Write(0);
        }
        else
        {
        LED1_Write(255);
        LED2_Write(255);
        LED3_Write(255);
        LED4_Write(255);
        LED5_Write(255);
        LED6_Write(255);
        LED7_Write(255);
        }
	}
		
		
}




	
/* [] END OF FILE */

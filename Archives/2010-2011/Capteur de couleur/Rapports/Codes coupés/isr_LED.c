/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START isr_LED_intc` */
#include "device.h"

uint16 demi_period[2];
int8 is_acquired[2];

/* `#END` */

/*******************************************************************************
* Function Name: isr_LED_Interrupt
********************************************************************************
* Summary:
*   The default Interrupt Service Routine for isr_LED.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
*
*
* Parameters:  
*
*
* Return:
*   void.
*
*******************************************************************************/
CY_ISR(isr_LED_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START isr_LED_Interrupt` */
	static uint8 color_L[3], color_R[3];
	static float coeff_L[3] = {1, 1, 1}, coeff_R[3] = {1, 1, 1};
	static T_sensors sensors;
	
	LED_change();
	
	acquisition(LEFT, color_L, coeff_L);
	acquisition(RIGHT, color_R, coeff_R);
	
	if (Counter_LED_ReadCounter() == 3) {
		// Aucune LED allumée : 4ème composante acquise
		
		if (is_acquired[LEFT] != 3) {
			if (CyPins_ReadPin(Button_etalonnage_L_0) != 0)
				// Si appui : étalonage (mettre blanc devant le capteur)
				etalonnage (LEFT, color_L, coeff_L);
			sensors.unit.left = convert_RGB_to_HSL(color_L);
		}
		if (is_acquired[RIGHT] != 3) {
			if (CyPins_ReadPin(Button_etalonnage_R_0) != 0)
				// Si appui : étalonage (mettre blanc devant le capteur)
				etalonnage (RIGHT, color_R, coeff_R);
			sensors.unit.right = convert_RGB_to_HSL(color_R);
		}
		is_acquired[LEFT] = 0;
		is_acquired[RIGHT] = 0;
		
		commande_moteurs(sensors);
	}

    /* `#END` */
}

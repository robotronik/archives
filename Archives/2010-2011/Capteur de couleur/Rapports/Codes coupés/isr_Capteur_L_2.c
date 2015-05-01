/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START isr_Capteur_L_2_intc` */
#include "Capteur.h"

/* `#END` */

/*******************************************************************************
* Function Name: isr_Capteur_L_2_Interrupt
********************************************************************************
* Summary:
*   The default Interrupt Service Routine for isr_Capteur_L_2.
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
CY_ISR(isr_Capteur_L_2_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START isr_Capteur_L_2_Interrupt` */
	capteur_L();

    /* `#END` */
}

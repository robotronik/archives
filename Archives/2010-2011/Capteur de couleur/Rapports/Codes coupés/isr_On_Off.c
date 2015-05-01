/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START isr_On_Off_intc` */
uint8 isFollowing = 0;

/* `#END` */

/*******************************************************************************
* Function Name: isr_On_Off_Interrupt
********************************************************************************
* Summary:
*   The default Interrupt Service Routine for isr_On_Off.
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
CY_ISR(isr_On_Off_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START isr_On_Off_Interrupt` */
	isFollowing = !isFollowing;

    /* `#END` */
}

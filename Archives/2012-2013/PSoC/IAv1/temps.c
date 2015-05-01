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

#include "temps.h"
#include "constantes.h"
#include "Timer_Fin.h"
#include "sharp.h"

volatile int secondesEcoulees = 0; 
volatile int secondesRestantes = 87; 

volatile int t45s = 0;
volatile int t45sN = 1;
volatile int t60s = 0;
volatile int t60sN = 1;

static unsigned int curCpt = 0;

void checkTemps(void)
{
	int dejaEnable = 0;
	PRINTF(" 1 Hz interupt\r");
	curCpt = Timer_Fin_ReadCounter(); // en 2 ms
	secondesRestantes = ((long int)curCpt/(long int)1000)*2;
	secondesEcoulees = TEMPS_FIN_S - secondesRestantes;
	
	if (secondesEcoulees >= 60)
	{
		t60s = 1;
		t60sN = 0;
	}
	else if (secondesEcoulees >= 45)
	{
		t45s = 1;
		t45sN = 0;
	}
	else if (secondesEcoulees >= 2)
	{
		if (!dejaEnable)
		{
			enableDetection();
			dejaEnable = 1;
		}
	}
	
	/*if (couleur == ROUGE)
	{
		if (missionCourante != BOUGIES && missionCourante != BOUGIES_FACILES)
		{
		
		}
	}*/
	
	PRINTF("sec restantes %d, sec ecoulees :%d\r", secondesRestantes, secondesEcoulees);

}

/* [] END OF FILE */

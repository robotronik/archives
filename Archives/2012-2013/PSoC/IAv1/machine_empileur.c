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
#include "comAsser.h"
#include "Depart.h"
#include "comXbee.h"
#include "LED_Debug.h"
#include "Empileur.h"
#include "Verre_Present.h"
#include "porte.h"
#include "Timer_Emp.h"
#include "TimerResetCtrl.h"

//#include "PWM_ServoDroitHaut.h"

enum StateEmpileur { ATT_MONTER, ATT_DESCENDRE, MONTER, DESCENDRE, LIBERER,  FIN_LIBER };
typedef enum StateEmpileur StateEmpileur;


volatile int finTimeEmp = 0;
int failSafeEmpEnable = 1;

extern int libererMAE;
extern int finLiberMAEN;

int nbVerreEmpile = 0;
int pileDe4 = 0;

static StateEmpileur curState = ATT_DESCENDRE, nextState = ATT_DESCENDRE;

void machineEmpileur(void)
{
	//static unsigned long attD = 0;
	//static unsigned long attM = 0;
	static int cptFinLiber = 0;
	static int cptFailSafe = 0;
	switch (curState)
	{
		case ATT_DESCENDRE:
			Empileur_Write(0);
			if (libererMAE)
			{
				finTimeEmp = 1;
				nextState = LIBERER;
			}
			else if(!Verre_Present_Read())
			{
				DEBUG_XBEE("___VERRE_D\r\n");
				nextState = MONTER;
				resetTimerEmpileur();
				Timer_Emp_Start();
			}
			else
				nextState = ATT_DESCENDRE;
			break;
		case ATT_MONTER:
			Empileur_Write(0xFF);
			if (libererMAE)
			{
				nextState = LIBERER;
				
				cptFailSafe = 0;
				
				resetTimerEmpileur();
				Timer_Emp_Start();
			}
			else if(!Verre_Present_Read() && nbVerreEmpile<3)
			{
				DEBUG_XBEE("___VERRE_M\r\n");
				nextState = DESCENDRE;
				cptFailSafe = 0;
				resetTimerEmpileur();
				Timer_Emp_Start();
			}
			else if ( Verre_Present_Read() && nbVerreEmpile <= 3 && cptFailSafe >= 5 )
			{
				if (!pileDe4 && failSafeEmpEnable)
				{
					PRINTF("FAIL SAFE !!!!!!\r");
					nextState = DESCENDRE;
					resetTimerEmpileur();
					Timer_Emp_Start();
					nbVerreEmpile--;
					cptFailSafe = 0;
				}
				else
				{
					cptFailSafe = 0;
					nextState = ATT_MONTER;
				}
				
			}
			else if(cptFailSafe == 0 && Verre_Present_Read())// si pas de verre et cpt==0
			{
				PRINTF("Dem cpt fail safe\r");
				resetTimerEmpileur();
				Timer_Emp_Start();
				cptFailSafe++;
				nextState = ATT_MONTER;
			}
			else if (finTimeEmp)
			{
				finTimeEmp = 0;
				cptFailSafe++;
				PRINTF("fin time fail safe %d\r",cptFailSafe);
				resetTimerEmpileur();
				Timer_Emp_Start();
				nextState = ATT_MONTER;
			}
			else
			{
				nextState = ATT_MONTER;
				if (nbVerreEmpile == 3 && !Verre_Present_Read())
				{
					if (!pileDe4)
						DEBUG_PRINTF("Pile de 4");
					pileDe4 = 1;
					cptFailSafe = 0;
				}
					
				
			}
			break;
		case DESCENDRE:
			Empileur_Write(0);

			if ( finTimeEmp )
			{
				DEBUG_XBEE("__EMPILE_D_\r\n");
				nextState = MONTER;

				finTimeEmp = 0;

				resetTimerEmpileur();
				Timer_Emp_Start();
			}
			else
				nextState = DESCENDRE;
			break;
		case MONTER:
			Empileur_Write(0xFF);
			if ( finTimeEmp )
			{
				nextState = ATT_MONTER;
				nbVerreEmpile++;
				DEBUG_XBEE("NB_VERRE++\r\n");
				finTimeEmp = 0;
			}
			else
				nextState = MONTER;
			break;
		case LIBERER:
			Empileur_Write(0);//descente
			if (finTimeEmp)
			{
				DEBUG_XBEE("_LIBERATION\r\n");
				ouvrirPorte();
				nextState = FIN_LIBER;
				libererMAE = 0;
				finLiberMAEN = 1;
				nbVerreEmpile = 0;
				pileDe4 = 0;
				finTimeEmp = 0;
				resetTimerEmpileur();
				Timer_Emp_Start();
			}
			else
				nextState = LIBERER;
			break;
		case FIN_LIBER :
			if (finTimeEmp && cptFinLiber < 1)
			{
				DEBUG_PRINTF("liber timer++");
				cptFinLiber++;
				nextState = FIN_LIBER;
				finTimeEmp = 0;
				resetTimerEmpileur();
				Timer_Emp_Start();
			}
			else if (finTimeEmp && cptFinLiber == 1)
			{
				DEBUG_XBEE("_FIN_LIBER\r\n");
				finTimeEmp = 0;
				fermerPorte();
				cptFinLiber = 0;
				finTimeEmp = 0;
				finLiberMAEN = 0;
				//libererMAE = 0;
				nextState = ATT_DESCENDRE;
			}
			else
			{
				nextState = FIN_LIBER;
			}
			break;
			
	}	
	curState = nextState;

}

void  enableFailSafeEmp(void)
{
	failSafeEmpEnable = 1;
}

void disableFailSafeEmp(void)
{
	failSafeEmpEnable = 0;
}

/* [] END OF FILE */

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
#include "device.h"

/*#include "UART_XBee.h"
#include "UART_Asser.h"*/
#include "comXbee.h"
#include "Depart.h"
#include "comAsser.h"
#include "machine.h"
#include "machine_go.h"
#include "machine_mind.h"
#include "machine_empileur.h"
#include "bougies.h"
#include "constantes.h"
#include "table.h"
#include "objectifs.h"
#include "forge.h"
#include "callage.h"
#include "cartographie.h"
#include "porte.h"
#include "sharp.h"

#include "gestionBras.h"
extern Mission missionCourante;
extern Mission missionReprise;

Couleur couleur;
volatile Pos robotNous;
volatile Pos robot2;
Pos pDepart = /*{2400, 1600, -90};*/{120, 550, 0};
//Pos pDepart = {LONGUEUR-760, HAUTEUR-195, -90};

extern volatile int requestGetXY;
extern volatile int requestGetAlpha;

void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

	Ballon_Write(0xFF);
	Empileur_Write(0);
	LED_Debug_Write(0);
	UART_XBee_Start();
	UART_XBee_RX_isr_Start();
	UART_Asser_Start();
	UART_Asser_RX_isr_Start();
	
	UART_Rpi_Start();
	UART_Rpi_RX_isr_Start();
	
	isr_Fin_Start();
	isr_Fin_Emp_Start();
	isr_Fin_Bras_Start();
	MuxSharp_Select(1);
	ADCSharp_Start();
	ADCSharp_StartConvert();
	PWM_ServoPorte_Start();
	BumperGaucheISR_Start();
	BumperDroitISR_Start();
	BumperGaucheFinISR_Start();
	BumperDroitFinISR_Start();

	
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
	
	CyDelay(1000);
	

	PRINTF("I'm_Ready.\r\n");
	PRINTF(" Reset status \r %x \r",CyResetStatus);


	/* INIT deuxieme phase */
	
	couleur = (Couleur)Couleur_Read(); //seul et unique fois ou la couleur est acquise
	DEBUG_PRINTF("Couleur : %d",couleur);
	
	/* SET de la position de l'asser */
	sendSetXY(pDepart.x, pDepart.y);
	CyDelay(50);
	sendSetAlpha(pDepart.alpha);
	CyDelay(100);
	getXY();
	CyDelay(100);
	getAlpha();
	CyDelay(100);
	
	//fermerPorte();
	/* CALLAGE */
	callageDepart();
	

	/* INIT troisème phase */
	PRINTF("Init des variables\r\n");;
	initCarto();
	initPiles();
	initGateau();
	initVerre();
	initMission2();
	//robotNous = pDepart;//DEBUG
	initObjectifs();
	
	//initMission(&missionCourante);
	//genererEvitementAstar(missionCourante.tabAction, pDepart, (Pos){2000,600,0} , (Pos){612,653,0});//test
	//missionCourante.numero++;//DEBUUG
	//forgerMissionCartoEvitementAstar(&missionReprise, pDepart, (Pos){920,1060,0});//DEBUG
	
	// sécurité pour sortir des cases
	disableDetection();
	
	if (couleur == ROUGE ) //pasque sinon y bouge :(
	{
		starterBrasGaucheHaut();	
		replierBrasGaucheHaut();
		starterBrasGaucheBas();
		replierBrasGaucheBas();
	}
	
	LED_Debug_Write(255); //On est ready
	PRINTF("READY");
    for(;;)
    {
		/* Place your application code here. */
		machineGo();
		machineMind();
		machineEmpileur();
    }
}

/* [] END OF FILE */

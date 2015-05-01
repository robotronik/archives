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

#include "init.h"
#include "main.h"
#include "asser.h"
#include "test_asser.h"
#include "servomoteur.h"
#include "figures.h"

#ifdef SIMU
void PSoC_main()
#else	// #ifndef COLLECTEUR
void main()
#endif	// #ifndef COLLECTEUR
{
	/* Purple :
	 * 		P    R
	 * 		^ y
	 * 		|  x  	sens trigo, 180° sur x
	 * 		0 ->
	 * 
	 * Red :
	 * 		P    R
	 * 		   y ^
	 * 		  x  |	sens horaire, 180° sur x
	 * 		  <- 0
	 */

	PSoCinit();
	
	while (Depart_Read())
		if (gamePhase == TEST_PHASE) boucle_test();
	CyDelay(200);
	while (!Depart_Read())
		if (gamePhase == TEST_PHASE) boucle_test();
	gamePhase = JEU;
	UART_XBee_PutString(" Preparation ");
	CyDelay(500);
	
	isRed = Couleur_Read();

#if DO_CALLE
	calleStart();
#else	// #if DO_CALLE
	if (isRed) sendRED();
	else sendPURPLE();
	
	// Avance à l'avant de la zone de départ
	sendXY_RAPIDE(300, 1750, PRECISION_XY);
	sendALPHA(180, PRECISION_ALPHA);
	waitDone(2);
#endif	// #if DO_CALLE
	UART_XBee_PutString("Pret ");
	
	while (Depart_Read());
	Timer_Fin_Start();
	isr_Fin_Start();
	UART_XBee_PutString("Go ");
	
	//T_Pos pos = updateAndWaitPos();
	
#if SERVO
	PWM_ServoDroit_Start();
	PWM_ServoGauche_Start();
#endif	// #if SERVO
	
	//testXav();
	//carres();
	//infini();
	testJul();
	while(1);
}

void fin_IRQ_Handler() {
#if SERVO
	PWM_ServoDroit_Stop();
	PWM_ServoGauche_Stop();
#endif	// #if SERVO
	
	UART_XBee_PutString("Fin du match ! ");
	
	UART_XBee_RX_isr_Stop();
	UART_Asser_Stop();
	UART_Asser_RX_isr_Stop();
	while (!(UART_XBee_ReadTxStatus() & UART_XBee_TX_STS_FIFO_EMPTY));
	UART_XBee_Stop();
	while(1);
}

/* [] END OF FILE */

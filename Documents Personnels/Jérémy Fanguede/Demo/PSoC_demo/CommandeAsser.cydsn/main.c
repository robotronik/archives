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
#include "main.h"
#include "messageAsser.h"
#include "servomoteur.h"
#include "test_asser.h"

boolean isPurple = TRUE;

void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	UART_XBee_Start();
#if IS_TX_ASSER_BYPASSED
	UART_XBee_TX_isr_Start();
#endif // IS_TX_ASSER_BYPASSED
	UART_Asser_Start();
	UART_Asser_RX_isr_Start();
#ifndef NO_SERVO
	PWM_ServoDroit_Start();
	PWM_ServoGauche_Start();
#endif // ! NO_SERVO

    CYGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */
	//UART_Asser_PutChar_Sniffed('a');
	//UART_Asser_PutChar('a');
#if ! IS_RX_ASSER_BYPASSED
	CyDelay(20);
	UART_XBee_PutChar('S');
#endif // ! IS_RX_ASSER_BYPASSED
	while (!isAsserReady) {
		sendTEST();
		CyDelay(10);
	}
#if ! IS_RX_ASSER_BYPASSED
	UART_XBee_PutChar('R');
#endif // ! IS_RX_ASSER_BYPASSED
	
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
	
#if IS_TESTING
	boolean hasColorChanged = TRUE;
	while(1) {
		if (isPurple != Couleur_Read()) {
			isPurple = Couleur_Read();
			hasColorChanged = TRUE;
		}
		if (hasColorChanged) {
			hasColorChanged = FALSE;
			if (isPurple) sendPURPLE();
			else sendRED();
		}
	
		while (nbAsserDone) {
			UART_XBee_PutChar('D');
			nbAsserDone --;
		}
#if ! IS_TX_ASSER_BYPASSED
		while (UART_XBee_GetRxBufferSize())
			test_message_processing();
#endif // ! IS_TX_ASSER_BYPASSED
	}
#endif // IS_TESTING
	
	// Avance à l'avant de la zone de départ
	sendXY_RAPIDE(300, 1750, 5);
	
	while (Depart_Read());
	
	isPurple = Couleur_Read();
	if (isPurple) sendPURPLE();
	else sendRED();
	
	T_Pos pos = getWaitPos();
	
	//sendXY_RAPIDE(pos.x + 2500, pos.y, 1);
	/*for (int i=0; i<10; i++) {
		sendALPHA(90, 10);
		sendALPHA(180, 10);
		sendALPHA(270, 10);
		sendALPHA(0, 10);
	}*/
	
	sendXY_RAPIDE(pos.x + 1000, pos.y + 750, 20);
	sendXY_RAPIDE(pos.x + 1500, pos.y + 750, 10);
	sendALPHA(180, 5);
	while(!nbAsserDone) CyDelay(10); nbAsserDone --;
	sendXY_COURBE(pos.x + 900, pos.y + 1350, 10);
	while(!nbAsserDone) CyDelay(10); nbAsserDone --;
	//sendXY_RAPIDE(pos.x + 400, pos.y, 20);
	//while(!nbAsserDone) CyDelay(10); nbAsserDone --;
	sendXY_RAPIDE(pos.x + 400, pos.y, 10);
	while(!nbAsserDone) CyDelay(10); nbAsserDone --;
	sendALPHA(180, 5);
	while(!nbAsserDone) CyDelay(10); nbAsserDone --;
	sendXY_RAPIDE(pos.x, pos.y, 10);
	while(!nbAsserDone) CyDelay(10); nbAsserDone --;
	while(!nbAsserDone) CyDelay(10); nbAsserDone --;
	while(!nbAsserDone) CyDelay(10); nbAsserDone --;
	/*sendALPHA(0, 5);
	sendXY_COURBE(pos.x, pos.y, 10);
	sendALPHA(0, 1);*/
	/*sendXY_RAPIDE(300, 1750, 10);
	while(!nbAsserDone) CyDelay(10); nbAsserDone --;*/
	//sendALPHA(180, 1);
	
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

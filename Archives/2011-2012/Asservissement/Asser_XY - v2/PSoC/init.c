/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 * ========================================
*/

#include "Boolean.h"
#include "init.h"
#ifdef COLLECTEUR
	#include "../../Collecteur/src/PSoCGlue.h"
#elif defined(SIMU)	// #ifndef COLLECTEUR
	#include "../../SimuAsserPSoC/src/PSoCGlue.h"
#else
	#include <device.h>
#endif	// #ifndef COLLECTEUR
#include "asser.h"

boolean isRed = FALSE;
phase_t gamePhase = ATTENTE;

inline void PSoCinit() {
	UART_XBee_Start();
	UART_XBee_RX_isr_Start();
	UART_Asser_Start();
	UART_Asser_RX_isr_Start();
	
    CYGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */
	
	CyDelay(20);
	UART_XBee_PutChar('S');
	connectAsser();
}
/* [] END OF FILE */

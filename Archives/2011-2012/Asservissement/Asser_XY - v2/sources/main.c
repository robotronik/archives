/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

#include "Boolean.h"
#include "init.h"
#include <p33FJ128MC802.h>
#include "asser.h"
#include "messageAsser.h"

// Ecrits par asser et interruption (mais bon quand même)
volatile boolean doitAttendre = TRUE;		// Echantilloneur

int main() {
	init();
	
	IEC3bits.PWM1IE = 1;		// Autorisation de l'interruption du timer
	while(1)
 	{
		while (doitAttendre);			// Attend l'interruption du timer
		doitAttendre = TRUE;
		
		asser();
		messages_processing();
	}
	
	return 0;
}

void __attribute__((interrupt, auto_psv)) _MPWM1Interrupt(void) {
	IFS3bits.PWM1IF = 0;	// On s'acquitte de l'interruption
	doitAttendre = FALSE;
}

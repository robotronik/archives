#include "main.h"
#include "debug.h"

#if !defined(COLLECTEUR) && !defined(SIMU)
void __attribute__((interrupt, auto_psv)) _AddressError(void)
{
	// Arrêt des moteurs
    PWM1CON1bits.PEN2H = 0;
    PWM1CON1bits.PEN2L = 0;
    
	while(1) {
		FLASH_LONG;
		FLASH_COURT;
	}
}

void __attribute__((interrupt, auto_psv)) _StackError(void)
{
	// Sans variable locale ni appel de fonction
	
	// Arrêt des moteurs
    PWM1CON1bits.PEN2H = 0;
    PWM1CON1bits.PEN2L = 0;
    
	while(1) {
		FLASH_LONG;
		FLASH_COURT;
		FLASH_COURT;
	}
}

void __attribute__((interrupt, auto_psv)) _MathError(void)
{
	// Arrêt des moteurs
    PWM1CON1bits.PEN2H = 0;
    PWM1CON1bits.PEN2L = 0;
    
	while(1) {
		FLASH_LONG;
		FLASH_COURT;
		FLASH_COURT;
		FLASH_COURT;
	}
}
#endif	// #if !defined(COLLECTEUR) && !defined(SIMU)

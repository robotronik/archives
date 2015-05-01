#include "timer.h"

void initTimer()
{
  //Activation du Timer PWM
  T1CONbits.TON=1; //Démarage du timer 
  T1CONbits.TCS = 0;
  T1CONbits.TSIDL=1;
  T1CONbits.TGATE=0;
  T1CONbits.TCKPS=0b00;
	
  IFS0bits.T1IF = 0;//Flag d'interuption à 0 
  IPC0bits.T1IP = 0b001;//Priorité 
  IEC0bits.T1IE=1; //Desactivation de l'interruption pour le timer 1
  PR1 = 0xFFFF;
// 2D : 40 khz


   T1CONbits.TON = 0;
}


void startTimer()
{
	// On remet a zero la valeur
	T1CONbits.TON = 0;
	TMR1 = 0;
	// On demarre
	T1CONbits.TON = 1;
}

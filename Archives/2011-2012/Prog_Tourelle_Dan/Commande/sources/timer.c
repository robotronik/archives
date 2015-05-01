#include "timer.h"

volatile unsigned char pauseTrame;
volatile unsigned char pauseCalcul;

void initTimer()
{
	INTCON1bits.NSTDIS = 1;
  //Activation du Timer trame US
  T1CONbits.TON=1; //Demarrage du timer 
  T1CONbits.TCS = 0;
  T1CONbits.TSIDL=1;
  T1CONbits.TGATE=0;
  T1CONbits.TCKPS=0b01;
	
  IFS0bits.T1IF = 0;//Flag d'interuption à 0 
  IPC0bits.T1IP = 0b011;//Priorite 
  IEC0bits.T1IE=0; //Activation de l'interruption pour le timer 1
  PR1=LONGUEUR_TRAME;	
  
  
  //Activation du timer de pause 
  T2CONbits.TON = 0;
  T2CONbits.TSIDL = 1;
  T2CONbits.TGATE= 0;
  T2CONbits.TCS= 0;
  T2CONbits.TCKPS= 0b01;
  T2CONbits.T32= 0;//16 bit counter
  
  IFS0bits.T2IF=0;
  IPC1bits.T2IP=0b011;
  IEC0bits.T2IE=0;
  PR2=0x0A48;
}

void waitForTrame()
{
	T1CONbits.TON = 1;
	IEC0bits.T1IE=1;
	TMR1 = 0;
	pauseTrame = 1;
	while(pauseTrame);
	IEC0bits.T1IE=0;
	T1CONbits.TON = 0;
}
void waitForCalcul()
{
	T2CONbits.TON = 1;
	IEC0bits.T2IE=1;
	TMR2 = 0;
	pauseCalcul = 1;
	while(pauseCalcul);
	IEC0bits.T2IE=0;
	T2CONbits.TON = 0;
}



//Timer de trame
void __attribute__((interrupt, auto_psv)) _T1Interrupt () 
{
	pauseTrame = 0;

	IFS0bits.T1IF = 0;
}

//Timer de pause
void __attribute__((interrupt, auto_psv)) _T2Interrupt () 
{
	pauseCalcul = 0;
	IFS0bits.T2IF = 0;
}


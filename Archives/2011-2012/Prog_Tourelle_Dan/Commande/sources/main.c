#include <p33FJ128MC802.h>
#include "init.h"
#include "uart.h"
#include "PWM.h"


int numTour;

int main()
{

	init();
	init_pwm();
	
	while(1)
	{
		
		if(toDoCalibrage)
		{
			sendSignalCalibrage(toDoCalibrage);
			// Fin de calibrage
			sendCalibrageDone();
			toDoCalibrage = 0;
		}
		if(semit)
		{
			emit();
			semit = 0;
		}
	}

	return 0;
}

void emit()
{
		// Emetteur 0
		emitUS_0();		
		waitForTrame();
		disableEmit();
		waitForCalcul(); //Attente récéption des US + temps calcul

		// Emetteur 1
		emitUS_1();		
		waitForTrame();
		disableEmit();
		waitForCalcul();

		// Emetteur 2
		emitUS_2();		
		waitForTrame();
		disableEmit();
		waitForCalcul();

		sendPosUpdated();
}


/*
void emit()
{
		int  i;

		TMR2=0;	//Reset du timer
		numTour=0;
		pause = 1;
		//On desactive les  tours
		disableEmit();
	    _RB5= 1;

	    //Emission des US ( 5ms )

	    IEC0bits.T1IE=1;
		while(pause);
		IEC0bits.T1IE=0;

		emitUS_0();
		//Attente de l'emission des US ( 25 ms )

		_RB5= 0;
		for(i=0; i <2 ;i++)
		{
			pause = 1;
			TMR2=0;
			while(pause);
		}


		TMR2=0;
		numTour=1;
		pause=1;
		disableEmit();
	    _RB5= 1;

		IEC0bits.T1IE=1;
		while(pause);
		IEC0bits.T1IE=0;
		_RB5= 0;
		emitUS_1();
		for(i=0; i <2 ;i++)
		{
			pause = 1;
			TMR2=0;
			while(pause);
		}


		TMR2=0;
		numTour=2;
		pause=1;

		disableEmit();
	    _RB5= 1;
		IEC0bits.T1IE=1;
		while(pause);
		IEC0bits.T1IE=0;
		_RB5= 0;
		emitUS_2();
		for(i=0; i <2 ;i++)
		{
			TMR2=0;
			pause = 1;
			while(pause);
		}
		disableEmit();
	
		sendPosUpdated();
}
*/

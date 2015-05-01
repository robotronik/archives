#include "calibreur.h"

void sendSignalCalibrage(char aCalibrer)
{
	// On envoie les trames en boucle basse frequence

	disableEmit();
	calibrage = 0b00;
	
	// On change la frequence du timer 1
	PR1 = PERIODE_TRAME_CALIB; // 0.5ms
	// On previent la tourelle que l'on veut calibrer
	switch(aCalibrer)
	{
		case 0b00 : return;
		case 0b01 : sendBeginCalibrage(0b100); break;	
		case 0b10 : sendBeginCalibrage(0b101); break;	
		case 0b11 : sendBeginCalibrage(0b111); break;
		default : return;
	}

	// DANGER : pas de timeout
	while(calibrage != aCalibrer)
	{
		// On envoie une trame toutes les ms
		emitCalibrage();
		waitForTrame();
		disableEmit();
		waitForTrame();
	}

}

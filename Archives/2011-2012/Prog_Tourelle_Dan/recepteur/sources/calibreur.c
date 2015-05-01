#include "calibreur.h"

volatile unsigned short seuil = 1335;

unsigned char getPourcentageRecu()
{
	unsigned int nombreOui = 0; 		// fais moi plaisir, vaux 60 :-)

	unsigned char isOui = 0;
	int i;
	for(i=0; i<120; i++)
	{
		// Attente 25us
		// Prise active
		
		if(isOui)
		{
			nombreOui++;
			isOui = 0;
		}	
	}
		 
	return nombreOui;
}

void calibrer()
{
	unsigned char decalage = 64;
	unsigned char pourcentageReceived = 0;
	// On prepare le timer
	int i;
	for(i=0; i<7; i++)
	{
		pourcentageReceived = getPourcentageRecu();
		if(pourcentageReceived < 50)
		{
			// On recoit rien, on baisse le seuil
			seuil -= decalage;
		}
		else if(pourcentageReceived > 70)
		{
			// On recoit trop, on augmente le seuil
			seuil += decalage;
		}
		else
			break;
		decalage = decalage >> 1;
	}

	// Envoi : fin de calibrage
}

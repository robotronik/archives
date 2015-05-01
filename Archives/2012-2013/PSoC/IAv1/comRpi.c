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
#include "comRpi.h"
#include "UART_Rpi.h"
#include "constantes.h"
#include "comAsser.h"
#include "UART_Rpi.h"
#include "bougies.h"



extern volatile Pos robotNous;
extern Couleur couleur;

char piBoot = 0;

extern Couleur couleur;

extern Bougie bougiesBas[NB_BOUGIES_BAS];
extern Bougie bougiesHaut[NB_BOUGIES_HAUT];


void sendRpi( unsigned short message)
{
	UART_Rpi_PutChar((uint8) (message >> 8));
	UART_Rpi_PutChar((uint8) message) ;
}

void sendRpi32( unsigned long message)
{
	UART_Rpi_PutChar((uint8)(message >> 24));
	UART_Rpi_PutChar((uint8)(message >> 16));
	UART_Rpi_PutChar((uint8)(message >> 8));
	UART_Rpi_PutChar((uint8) message) ;
}



void sendRpiXY(int x, int y)
{
	TramePi32 t;
	t.tpos.codet = CODE_XY;
	t.tpos.x = x;
	t.tpos.y = y;
	sendRpi32(t.l);

}


void sendRpiTirette()
{
	TramePi t;
	t.couleur.couleur = CODE_TIRETTE;
	UART_Rpi_PutChar(t.c);
	DEBUG_PRINTF("SendPiTirette");
}

void sendRpiCouleur()
{
	TramePi t;

	t.d.val = 0;
	t.couleur.codet = CODE_COULEUR_PI;
	t.couleur.couleur = couleur;
	UART_Rpi_PutChar(t.c);
	DEBUG_PRINTF("SendCouleur %x \r",t.c);

}



void UART_Rpi_RX_Handler(void)
{
	static int cpt = 0;
	
	unsigned char inMessage = UART_Rpi_ReadRxData();
	//UART_XBee_PutChar(inMessage);//debug
	DEBUG_PRINTF("RecuRpi : %x ",inMessage);
	TramePi trame;
	
	if ( cpt == 0 )
	{
		trame.c = inMessage;
		switch (trame.d.codet)
		{
			case CODE_GATEAU :
				PRINTF("Code Gateau recu\r");
				
				/****Constante*****/
				bougiesBas[0].c = BLEU;
				bougiesBas[4].c = BLANC;
				bougiesBas[5].c = BLANC;
				bougiesBas[6].c = BLANC;
				bougiesBas[7].c = BLANC;
				bougiesBas[11].c = ROUGE;
				
				bougiesHaut[0].c = BLEU ;
				bougiesHaut[7].c = ROUGE;
				
				
				/****Cas bleu*****/		
				if(couleur == BLEU)
				{				
					//Les bougies que nous avons recu 
					bougiesBas[1].c = trame.mire.M4;
					bougiesBas[2].c = trame.mire.M5;
					bougiesBas[3].c = trame.mire.M6;
					
					bougiesBas[10].c =!trame.mire.M4;
					bougiesBas[9].c = !trame.mire.M5;
					bougiesBas[8].c = !trame.mire.M6;
					
					//Bougies du haut 
					bougiesHaut[1].c = trame.mire.M1;
					bougiesHaut[2].c = trame.mire.M2;
					bougiesHaut[3].c = trame.mire.M3;
					
					
					bougiesHaut[6].c = !trame.mire.M1;
					bougiesHaut[5].c = !trame.mire.M2;
					bougiesHaut[4].c = !trame.mire.M3;
				
				}
				/****Cas rouge****/
				else
				{
					
					
					//Les bougies que nous avons recu 
					bougiesBas[1].c = !trame.mire.M4;
					bougiesBas[2].c = !trame.mire.M5;
					bougiesBas[3].c = !trame.mire.M6;
					
					bougiesBas[10].c =trame.mire.M4;
					bougiesBas[9].c = trame.mire.M5;
					bougiesBas[8].c = trame.mire.M6;
					
					//Bougies du haut 
					bougiesHaut[1].c = !trame.mire.M1;
					bougiesHaut[2].c = !trame.mire.M2;
					bougiesHaut[3].c = !trame.mire.M3;
					
					
					bougiesHaut[6].c = trame.mire.M1;
					bougiesHaut[5].c = trame.mire.M2;
					bougiesHaut[4].c = trame.mire.M3;
				
				}
				PRINTF("@@@@@ Couleurs des mires @@@@@\r");
				PRINTF("           %d %d %d\r",bougiesHaut[1].c,bougiesHaut[2].c,bougiesHaut[3].c)
				PRINTF("         %d %d %d\r",bougiesBas[1].c,bougiesBas[2].c,bougiesBas[3].c);
				
				
			break;
			
			case CODE_COULEUR_PI : //Le Rpi à booter \o/ 
                                   //                 |
				PRINTF("Le Pi a booter \r");/*       / \   /  */
				
				sendRpiCouleur();
			break;
		}
	}
	/*
	else if ( cpt == 1)
	{
		t16.trShort.oX = inMessage;
		t32.tr4Char.oXoo = inMessage;
		switch(messageType)
		{
			case CODE_GET_ALPHA :
				cpt = 0;
				if(t16.trAlpha.signe ==1)
					//posAlpha = -t16.trAlpha.alpha;
					robotNous.alpha = -t16.trAlpha.alpha;
				else
					//posAlpha = t16.trAlpha.alpha;
					robotNous.alpha = t16.trAlpha.alpha;
				DEBUG_PRINTF("GetAlpha : %d",robotNous.alpha);
			break;
			case CODE_GET_XY :
				cpt++;
			break;
		}
	}
	else if(cpt == 2)
	{
		cpt++;
		t32.tr4Char.ooXo = inMessage;
	}
	else
	{
		cpt = 0;
		t32.tr4Char.oooX = inMessage;
		switch(messageType)
		{
			case CODE_GET_XY :
				//posX = t32.tpos.x;
				//posY = t32.tpos.y;
				robotNous.x = t32.tpos.x;
				robotNous.y = t32.tpos.y;
				DEBUG_PRINTF("GetXY : %d,%d",robotNous.x,robotNous.y);
			break;
		}
	}
	*/
	
}


/* [] END OF FILE */

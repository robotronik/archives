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
#include "UART_XBee_RX_isr.h"
#include "UART_Xbee.h"
#include "constantes.h"
#include "comXbee.h"
#include "comAsser.h"
#ifdef DEBUG_MODE
char debugBuff[100];
#endif

extern Couleur couleur;
extern volatile Pos robotNous;
extern volatile Pos robot2;



void sendXBee( unsigned short message)
{
	UART_XBee_PutChar((uint8) (message >> 8));
	UART_XBee_PutChar((uint8) message) ;
	//DEBUG_PRINTF("%x",message);
}

void sendXBee32( unsigned long message)
{
	UART_XBee_PutChar((uint8)(message >> 24));
	UART_XBee_PutChar((uint8)(message >> 16));
	UART_XBee_PutChar((uint8)(message >> 8));
	UART_XBee_PutChar((uint8) message) ;
	//DEBUG_PRINTF("%x%x",(int)(message>>8),(int)(message));
}
void sendTiretteRobot2()
{
	TrXbee d;
	d.d.codet =  CODE_ROBOT2_TIRETTE;
	UART_XBee_PutChar((uint8)d.c);
}

void sendCouleurRobot2()
{
	TrXbee c;
	c.couleur.codet = CODE_ROBOT2_COULEUR;
	c.couleur.couleur = couleur;
	UART_XBee_PutChar((uint8)c.c);
}

void getRobot2Pos()
{
	TrXbee d;
	d.d.codet = CODE_ROBOT2_GET_XY;
	UART_XBee_PutChar((uint8)d.c);
}
void sendRobot2Pos()
{
	Trame32 t;
	t.tpos.y = robotNous.y;
	t.tpos.x =robotNous.x;
	
	t.tpos.codet = CODE_ROBOT2_XY;
	
	sendXBee32(t.l);
	
}

void sendRobot2Fin()
{
	TrXbee t;
	t.d.codet = CODE_FIN;
	
	UART_XBee_PutChar(t.c); 
	
}

void UART_XBee_RX_Handler(void)
{
	unsigned char inMessage = UART_XBee_ReadRxData();
	
	static int cpt = 0;
    static unsigned char messageType;
    TrXbee t;
    static Trame32 t32;

            
    if (cpt == 0)
    {
        t.c = inMessage;
        t32.tr4Char.Xooo = inMessage;
        switch (t.d.codet)
        {
        case CODE_ROBOT2_XY:
            cpt++;
            messageType = CODE_ROBOT2_XY;
            break;
			
		case CODE_ROBOT2_GET_XY:
		    //On envoie notre position 
			sendRobot2Pos();
			break;
        }
    }
    else if (cpt == 1)
    {
        t32.tr4Char.oXoo = inMessage;
        switch (messageType)
        {
        case CODE_ROBOT2_XY:
            cpt++;
            break;
        }
    }
    else if (cpt == 2)
    {
        cpt++;
        t32.tr4Char.ooXo = inMessage;
    }
    else
    {
        cpt = 0;
        t32.tr4Char.oooX = inMessage;
        switch (messageType)
        {
        case CODE_ROBOT2_XY:

		   //On recoit la position du robot 2
		   robot2.x = t32.tpos.x;
		   robot2.y = t32.tpos.y;
		   
		   //PRINTF("Position du robot2 recu : %d %d \r", robot2.x, robot2.y);
           break;
        }
    }
	
	
}

/* [] END OF FILE */

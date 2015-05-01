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

#include <device.h>
#include "comXBee.h"
#include "comAsser.h"
#include "global.h"

extern Couleur g_couleur;
extern volatile Position posRobot;
extern volatile Position posRobot2;

void printXBee(char* s, int t)
{
    int i;
    UART_XBee_PutChar(0x0D);
    for(i=0;i<t;i++)
    {
        UART_XBee_PutChar((uint8) s[i]);
    }
}

void printIntXBee(int c)
{
    if(c<0){
        UART_XBee_PutChar('-');
        printIntXBee(-c);
    }
    if(c == 0)
        return;
    else
    {
        printIntXBee(c/10);
        UART_XBee_PutChar((c%10)+0x30);
        return;
    }
}
        
void sendXBee( unsigned short message)
{
	UART_XBee_PutChar((uint8) (message >> 8));
	UART_XBee_PutChar((uint8) message) ;
}

void sendXBee32( unsigned long message)
{
	UART_XBee_PutChar((uint8)(message >> 24));
	UART_XBee_PutChar((uint8)(message >> 16));
	UART_XBee_PutChar((uint8)(message >> 8));
	UART_XBee_PutChar((uint8) message) ;
}

void sendTiretteRobot2()
{
	TrXbee d;
	d.d.codet =  CODE_ROBOT2_TIRETTE;
	UART_XBee_PutChar((uint8)d.c);
    //UART_XBee_PutChar(CODE_ROBOT2_TIRETTE);
}

void sendCouleurRobot2()
{
	TrXbee c;
	c.couleur.codet = CODE_ROBOT2_COULEUR;
	c.couleur.couleur = g_couleur;
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
	t.tpos.y = posRobot.y;
	t.tpos.x =posRobot.x;
	
	t.tpos.codet = CODE_ROBOT2_XY;
	
	sendXBee32(t.l);
	
}

void sendRobot2Fin()
{
	TrXbee t;
	t.d.codet = CODE_FIN;
	
	UART_XBee_PutChar(t.c); 
	
}

void XBee_Handler()
{
    unsigned char inMessage = UART_XBee_ReadRxData();
	static int cpt = 0;
    static unsigned char messageType;
    TrXbee t;
    static Trame32 t32;

    //debug
    sendXBee(inMessage);
    
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
        default :
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
		   posRobot2.x = t32.tpos.x;
		   posRobot2.y = t32.tpos.y;
           break;
        }
    }	
}


/* [] END OF FILE */

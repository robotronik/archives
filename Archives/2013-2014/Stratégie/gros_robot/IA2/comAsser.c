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
#include "comAsser.h"
#include <device.h>
#include "UART_Asser.h"
#include "global.h"
#include <stdio.h>

extern volatile int doneAsser;
extern int actionTerminee;
extern volatile Position posRobot;
extern Couleur g_couleur;

extern volatile int requestGetAlpha;
extern volatile int requestGetXY;

void resetDoneAsser(void)
{
	doneAsser = 0;
}

void sendAsser( unsigned short message)
{
	UART_Asser_PutChar((uint8) (message >> 8));
	UART_Asser_PutChar((uint8) message) ;
	//DEBUG_PRINTF("%x",message);
}

void sendAsser32( unsigned long message)
{
	UART_Asser_PutChar((uint8)(message >> 24));
	UART_Asser_PutChar((uint8)(message >> 16));
	UART_Asser_PutChar((uint8)(message >> 8));
	UART_Asser_PutChar((uint8) message) ;
	//DEBUG_PRINTF("%x%x",(int)(message>>8),(int)(message));
}

void sendDelta( short delta )
{
	TrameAsser t;
	if (delta < 0)
	{
		t.trDelta.delta = -delta;
		t.trDelta.signe = 1;
	}
	else
	{
		t.trDelta.delta = delta;
		t.trDelta.signe = 0;
	}
	t.trDelta.codet = CODE_DELTA;
	resetDoneAsser();
	sendAsser(t.us);
	//DEBUG_PRINTF("sendDelta %d",delta);
}

void sendAlpha( short alpha )
{
	TrameAsser t;
	if (alpha < 0)
	{
		t.trAlpha.alpha = -alpha;
		t.trAlpha.signe = 1;
	}
	else
	{
		t.trAlpha.alpha = alpha;
		t.trAlpha.signe = 0;
	}
	t.trAlpha.codet = CODE_ALPHA;
	resetDoneAsser();
	sendAsser(t.us);
	//DEBUG_PRINTF("sendAlpha %d",alpha);
}

void sendAlphaAbs( short alpha )
{
	TrameAsser t;
	if (alpha < 0)
	{
		t.trAlpha.alpha = -alpha;
		t.trAlpha.signe = 1;
	}
	else
	{
		t.trAlpha.alpha = alpha;
		t.trAlpha.signe = 0;
	}
	t.trAlpha.codet = CODE_ALPHA_ABS;
	resetDoneAsser();
	sendAsser(t.us);
	//DEBUG_PRINTF("sendAlphaAbs %d",alpha);
}

void sendStop(void)
{
	TrameAsser t;
	t.trShort.Xo = CODE_STOP;
	
	sendAsser(t.us);
	CyDelay(10);
//	DEBUG_PRINTF("#####sendStop#####");
}

void getXY(void)
{
	TrameAsser t;
	t.trGenShort.codet = CODE_GET_XY;
	requestGetXY = 1;
	//DEBUG_PRINTF("getXYRequest");
	sendAsser(t.us);
	
	
}

void sendSetXY(int x, int y)
{
	Trame32 t;
	t.tpos.codet = CODE_SET_XY;
	t.tpos.y = y;
	t.tpos.x = x;
	sendAsser32(t.l);
	//DEBUG_PRINTF("sendSetXY:%d,%d",x,y);
}

void sendSetAlpha(short alpha)
{
	TrameAsser t;
	if (alpha < 0)
	{
		t.trAlpha.alpha = -alpha;
		t.trAlpha.signe = 1;
	}
	else
	{
		t.trAlpha.alpha = alpha;
		t.trAlpha.signe = 0;
	}
	t.trAlpha.codet = CODE_SET_ALPHA;
	sendAsser(t.us);
//	DEBUG_PRINTF("sendSetAlpha:%d",alpha);
}

void getAlpha(void)
{
	TrameAsser t;
	t.trGenShort.codet = CODE_GET_ALPHA;
	requestGetAlpha = 1;
	//DEBUG_PRINTF("getAlphaRequest");
	sendAsser(t.us);
	
	
}

void sendPresGros(void)
{
	TrameAsser t;
	t.trGenShort.codet = CODE_PRES_GROS;
	sendAsser(t.us);
//	DEBUG_PRINTF("sendPresGros");
}

void sendPresPrecis(void)
{
	TrameAsser t;
	t.trGenShort.codet = CODE_PRES_PRECIS;
	sendAsser(t.us);
//	DEBUG_PRINTF("sendPresPrecis");
}

void sendFreeWheel(void)
{
	TrameAsser t;
	t.trGenShort.codet = CODE_FREE_WHEEL;
	sendAsser(t.us);
//	DEBUG_PRINTF("sendFreeWheel");
}

void UART_Asser_RX_Handler(void)
{
	static int cpt = 0;
	static unsigned char messageType;
	unsigned char inMessage = UART_Asser_ReadRxData();
	static TrameAsser t16;
	static Trame32 t32;
    
    //UART_XBee_PutChar(inMessage);//debug
	//DEBUG_PRINTF("RecuAsser : %x",inMessage);
	
	if ( cpt == 0 )
	{
		t16.trShort.Xo = inMessage;
		t32.tr4Char.Xooo = inMessage;
		switch (t16.trGenShort.codet)
		{
			case (DONE_ASSER) :
				//UART_XBee_PutString(" Fin Asser Vrai");
				doneAsser = 1;
                actionTerminee = 1;
				messageType = DONE_ASSER;
//				DEBUG_PRINTF("DoneAsser");
			break;
			/*case (CODE_DERAPAGE) :
				DEBUG_PRINTF("Derapage");
				derapageN = 0;
			break;*/
			case CODE_GET_ALPHA :
				cpt++;
				messageType = CODE_GET_ALPHA;
			break;
			case CODE_GET_XY :
				cpt++;
				messageType = CODE_GET_XY;
			break;
		}
	}
	else if ( cpt == 1)
	{
		t16.trShort.oX = inMessage;
		t32.tr4Char.oXoo = inMessage;
		switch(messageType)
		{
			case CODE_GET_ALPHA :
				cpt = 0;
				
				if(t16.trAlpha.signe ==1)
					posRobot.alpha = -t16.trAlpha.alpha;
			    else
					posRobot.alpha = t16.trAlpha.alpha;
				requestGetAlpha = 0;
	//			PRINTF("GetAlpha : %d\r",posRobot.alpha);
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
					posRobot.x = t32.tpos.x;
					posRobot.y = t32.tpos.y;
				requestGetXY = 0;
//				PRINTF("GetXY : %d,%d\r",posRobot.x,posRobot.y);
				//On envoit notre position au Pi 
//				sendRpiXY(posRobot.x,posRobot.y );
			break;
		}
	}
	
	
}

void waitRequestGetAlphaSafe(void)
{
	int i =  0;
	do {
		if (!requestGetAlpha) // si tres rapide
			return;
		CyDelay(2); // on atends un peu
		if (!requestGetAlpha)
			return;
		else
//			PRINTF("get Alpha long /!\\\r");
		CyDelay(3);
		if (!requestGetAlpha)
			return;
		else
		{
			CyGlobalIntDisable; 
			getAlpha();
			CyGlobalIntEnable; 
			
//			PRINTF("get Alpha trop long -> request\r");
		}
		i++;
		if ( i > 3)
			return;
	} while(requestGetAlpha);
}

void waitRequestGetXYSafe(void)
{
	int i =  0;
	do {
		if (!requestGetXY) // si tres rapide
			return;
		CyDelay(4); // on atends un peu
		if (!requestGetXY)
			return;
		else
//			PRINTF("get XY long /!\\\r");
		CyDelay(4);
		if (!requestGetXY)
			return;
		else
		{
			CyGlobalIntDisable; 
			getXY();
			CyGlobalIntEnable; 
//			PRINTF("get XY trop long -> request\r");
		}
		i++;
		if ( i > 3)
			return;
	} while(requestGetXY);
}

// should not be used
void waitDoneAsser(void)
{

	while(!doneAsser);
	doneAsser = 0;
	
}

/* [] END OF FILE */

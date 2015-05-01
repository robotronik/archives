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
#include "comXBee.h"
#include "global.h"
#include <device.h>

extern volatile int actionTerminee;
extern volatile int requestGetXY;
extern volatile int requestGetAlpha;

extern Position posRobot;

void resetActionTerminee(void)
{
	actionTerminee = 0;
}

void sendAsser( unsigned short message)
{
	UART_Asser_PutChar((uint8) (message >> 8));
	UART_Asser_PutChar((uint8) message) ;
}

void sendAsser32( unsigned long message)
{
	UART_Asser_PutChar((uint8)(message >> 24));
	UART_Asser_PutChar((uint8)(message >> 16));
	UART_Asser_PutChar((uint8)(message >> 8));
	UART_Asser_PutChar((uint8) message) ;
}

void sendDelta( short delta )
{
	Trame16 t;
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
	resetActionTerminee();
	sendAsser(t.us);
}

void sendAlpha( short alpha )
{
	Trame16 t;
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
	resetActionTerminee();
	sendAsser(t.us);
}

void sendAlphaAbs( short alpha )
{
	Trame16 t;
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
	resetActionTerminee();
	sendAsser(t.us);
}

void sendStop(void)
{
	Trame16 t;
	t.trShort.Xo = CODE_STOP;
	sendAsser(t.us);
	CyDelay(10);
}

void getXY(void)
{
	Trame16 t;
	t.trGenShort.codet = CODE_GET_XY;
	requestGetXY = 1;
	sendAsser(t.us);
	
	
}

void sendSetXY(int x, int y)
{
	Trame32 t;
	t.tpos.codet = CODE_SET_XY;
	t.tpos.y = y;
	t.tpos.x = x;
	sendAsser32(t.l);
}

void sendSetAlpha(short alpha)
{
	Trame16 t;
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
}

void getAlpha(void)
{
	Trame16 t;
	t.trGenShort.codet = CODE_GET_ALPHA;
	requestGetAlpha = 1;
	sendAsser(t.us);
	
	
}

void Asser_Handler(void)
{
	static int cpt = 0;
	static unsigned char messageType;
	unsigned char inMessage = UART_Asser_ReadRxData();
	static Trame16 t16;
	static Trame32 t32;
    
    //sendXBee('A');
    
	if ( cpt == 0 )
	{
		t16.trShort.Xo = inMessage;
		t32.tr4Char.Xooo = inMessage;
		switch (t16.trGenShort.codet)
		{
			case (DONE_ASSER) :
                actionTerminee = 1;
				messageType = DONE_ASSER;
                //printXBee("==========DONE_ASSER============",32);
			break;
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
				
				if(t16.trAlpha.signe ==1){
					posRobot.alpha = -t16.trAlpha.alpha;
                }
			    else{
					posRobot.alpha = t16.trAlpha.alpha;
                }
				requestGetAlpha = 0;
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
				posRobot.x = t32.tpos.x;
				posRobot.y = t32.tpos.y;
				requestGetXY = 0;
			break;
		}
	}	
}

void waitRequestGetAlphaSafe(void)
{
	int i =  0;
	do 
    {
		if (!requestGetAlpha)
			return;
            
		CyDelay(2);
        
		if (!requestGetAlpha)
			return;
		else
		CyDelay(3);
        
		if (!requestGetAlpha)
			return;
		else
		{
			CyGlobalIntDisable; 
			getAlpha();
			CyGlobalIntEnable; 
		}
		i++;
		if ( i > 3)
			return;
	} while(requestGetAlpha);
}

void waitRequestGetXYSafe(void)
{
	int i =  0;
	do 
    {
		if (!requestGetXY)
			return;
            
		CyDelay(4);
        
		if (!requestGetXY)
			return;
		else
		    CyDelay(4);
            
		if (!requestGetXY)
			return;
		else
		{
			CyGlobalIntDisable; 
			getXY();
			CyGlobalIntEnable; 
		}
        
		i++;
		if ( i > 3)
			return;
	} while(requestGetXY);
}

void waitDoneAsser(void)
{

	while(!actionTerminee);
	actionTerminee = 0;
	
}
/* [] END OF FILE */

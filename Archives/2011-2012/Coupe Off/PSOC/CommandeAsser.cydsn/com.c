#include "com.h"
#include "cst.h"


void sendAsser(unsigned char moteur, char vitesse) {
	vitesse = vitesse | (moteur<<7);
	UART_XBee_PutChar(vitesse);
	UART_Asser_PutChar(vitesse);
	
	
}
void commande(char m)
{
	UART_Asser_PutChar(m);
}


/*
void commande(char m)
{
	static volatile unsigned char nbOctRestant=1;
	static char message;
	
	if (nbOctRestant) {
		nbOctRestant --;
		message=m;
		//UART_XBee_PutChar(0x1);
		

	}
	else {
		//UART_XBee_PutChar(0x2);
		switch(message) {
		case PWMG:
		{
			sendAsser(0,m);
			nbOctRestant=1;
			
			break;
			}
		case PWMD :
		{
			sendAsser(1, m);
			nbOctRestant=1;
			break;
			}
			
		case SGP :
		{
			if(PWM_ServoGauche_ReadCounter()+PASSERVO > 60000)
			{
				PWM_ServoGauche_WriteCounter(65000);
			
			}
			else if(PWM_ServoGauche_ReadCounter()+PASSERVO < 25000)
			{
				PWM_ServoGauche_WriteCounter(25000);
			}
			else
			{
				PWM_ServoGauche_WriteCounter ( PASSERVO + PWM_ServoGauche_ReadCounter());
			}
			break;
		}
		case SGM : // TODO: A corriger (moins)
		{
			if(PWM_ServoGauche_ReadCounter()+PASSERVO > 60000)
			{
				PWM_ServoGauche_WriteCounter(65000);
			
			}
			else if(PWM_ServoGauche_ReadCounter()+PASSERVO < 25000)
			{
				PWM_ServoGauche_WriteCounter(25000);
			}
			else
			{
				PWM_ServoGauche_WriteCounter ( PASSERVO + PWM_ServoGauche_ReadCounter());
			}
			break;
		}
			
			
		/*case SDP :
			sendAsser(1, m);
			nbOctRestant=1;
			break;
			
		case SDM :
			sendAsser(1, m);
			nbOctRestant=1;
			break;
			
			
		default :
			nbOctRestant=1;
		break;
		
		
		
		
	    }

	}
}
*/


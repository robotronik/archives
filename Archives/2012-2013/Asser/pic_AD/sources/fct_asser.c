#include "main.h"
#include "myMath.h"
#include "motors.h"
#include "message.h"
#include <math.h>

extern long int AD_alphaTotal;
extern long int AD_alpha;

extern volatile long posX;
extern volatile long posY;

void majAlpha(void)
{
	/*AD_alphaTotal += (AD_alpha*(TICK_TOUR/(2.0*3.141592))); //en ticks 
	AD_alphaTotal %= (long int)TICK_TOUR;*/
	AD_alphaTotal += AD_alpha;
	AD_alphaTotal %= (long int)TICK_TOUR;
}	

void majXYAngle(long int alphaParam, long int newDelta)	
{
	static long int precDelta = 0;
	static double dX = 0;
	static double dY = 0;
	double pX, pY;
	double alphad = (double) alphaParam;
	long int alphac = alphaParam;
	long int dDelta = newDelta - precDelta;
	precDelta = newDelta;
	
/*	alphac %= (long int)TICK_TOUR;
	alphac /= 360;
	alphac *= (unsigned long int)TICK_TOUR;
	alphad = (double)alphac;
	alphad = 3.14*alphad/180.;*/
	
	//alphad = alphad%(double) TICK_TOUR;

	alphad = alphad/((double) TICK_TOUR)*(2.*3.14);
	
	dX += cos(alphad)*(double)dDelta;
    dY += sin(alphad)*(double)dDelta;

    //conversion de la position en mm

    pX = dX*TICKS_TO_MM;
    pY = dY*TICKS_TO_MM;
    
    posX = (long) pX;
    posY = (long) pY;

}			

void maj_erreurs( long int erreurs[4],long int erreur)
{
	erreurs[3] = erreurs[2]; //plus ancien
	erreurs[2] = erreurs[1];
	erreurs[1] = erreurs[0];
	erreurs[0] = erreur;// plus recent
}

long int deriv_erreurs(long int erreurs[4])
{
  long int derivee=0;
  
  derivee = (erreurs[0]-erreurs[1]);

  return derivee;
}

long int rampe(long int vcommande, long int prec)
{
	long int commande = vcommande;
	if (AD_abs(commande) > (AD_abs(prec) + ACCELERATION_MAX)) //acceleration
	{
		if (commande > 0) //en avant
			commande = prec + ACCELERATION_MAX;
		else //en arriere
			commande = prec - ACCELERATION_MAX;
	}		
	else if (AD_abs(commande) < (AD_abs(prec) - COEFF*ACCELERATION_MAX)) //decceleration
	{
		if (commande > 0) //en avant
			commande = prec - COEFF*ACCELERATION_MAX;
		else //en arriere
			commande = prec + COEFF*ACCELERATION_MAX;
	}		
				
	//Il ne faut pas oublier de forcer le max speed dans l'asser même si set_speed le fait déja ( augmente le temps de décleration )
	if(commande > MAX_SPEED) commande = MAX_SPEED;
	else if ( commande < -MAX_SPEED ) commande = -MAX_SPEED;
	return commande;
}

void AD_motors_set_speed(short speedL, short speedR)
{
	short commandeL = speedL;
	short commandeR = speedR;
	if ( speedL > 0)
	{
		if ( speedL < MIN_SPEED )
	  	{
	      commandeL = MIN_SPEED;
	  	}
		else if (speedL > MAX_SPEED )
		{
			commandeL = MAX_SPEED;   
		}
	}
    else if(speedL < 0)
	{
	  if ( -speedL < MIN_SPEED )
	  {
	       commandeL = -MIN_SPEED;
	  }
	  else if (-speedL > MAX_SPEED )
	  {
		 	commandeL = -MAX_SPEED;   
	  }
	}
	
	if ( speedR > 0)
	{
		if ( speedR < MIN_SPEED )
	  	{
	      commandeR = MIN_SPEED;
	  	}
		else if (speedR > MAX_SPEED )
		{
			commandeR = MAX_SPEED;   
		}
	 } 
    else if(speedR < 0)
	{
	  if ( -speedR < MIN_SPEED )
	  {
	       commandeR = -MIN_SPEED;
	  }
	  else if (-speedR > MAX_SPEED )
	  {
		 	commandeR = -MAX_SPEED;   
	  }
	}
	motors_set_speed (commandeL,commandeR);
}	


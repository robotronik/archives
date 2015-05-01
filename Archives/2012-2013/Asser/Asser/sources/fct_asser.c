#include "main.h"
#include "motors.h"
#include "message.h"
#include "asser.h"
#include <math.h>

extern long int AD_alphaTotal;
extern long int AD_alpha;
extern volatile double alphaRad; //^postion abs en angle en radian

extern volatile long posX;
extern volatile long posY;

volatile double dXdouble = 0;
volatile double dYdouble = 0;

extern volatile AD_K kP_delta;
extern volatile AD_K kD_delta;

extern volatile AD_K kP_alpha;
extern volatile AD_K kD_alpha;


extern volatile long int acceleration_max;

AD_K P_delta_precis = { 8, 20 }; //5mm et 1�
AD_K D_delta_precis = { 500, 400 };
AD_K P_alpha_precis = { 100, 15};
AD_K D_alpha_precis = { 100 , 1000 };
long int acceleration_max_precis = 15;

AD_K P_delta_gros = { 10, 50 };//2,1
AD_K D_delta_gros = {0, 0 };
AD_K P_alpha_gros = { 50, 10};
AD_K D_alpha_gros = { 0 , 10 };
long int acceleration_max_gros = 5;//20


void setPrecis()//Pr�cision � 5mm et 1�
{
    kP_delta = P_delta_precis ;
    kD_delta = D_delta_precis;

    kP_alpha = P_alpha_precis;
    kD_alpha  = D_alpha_precis;

    acceleration_max = acceleration_max_precis;
}

void setGros() // Pr�cision � 2cm et 3�
{
    kP_delta = P_delta_gros ;
    kD_delta = D_delta_gros;

    kP_alpha = P_alpha_gros;
    kD_alpha  = D_alpha_gros;

    acceleration_max = acceleration_max_gros;
}

void majXYAngle(double dTheta, double dDelta)
{
    static double cosTheta;
    static double sinTheta;
    alphaRad += atan(dTheta);

    // modulo dans -pi;pi
    if (alphaRad > PI)
        alphaRad -= DEUX_PI;
    else if (alphaRad <= -PI)
       	alphaRad += DEUX_PI;

    cosTheta = cos(alphaRad);
    sinTheta = sin(alphaRad);

    dXdouble += dDelta*cosTheta;
    dYdouble += dDelta*sinTheta;

    AD_alphaTotal = alphaRad*(TICK_TOUR/DEUX_PI);


#ifdef NULL
    double dX = 0;
    double dY = 0;
    //double alphad;
    double alphaTmp = dAlpha/(TICK_TOUR/3.1415926535897932384626433832795);//dAlpha/D 
    alphaRad += atan(alphaTmp);
    /*double alphaTmp = dAlpha*(2.0*3.1415926535897932384626433832795)/((double)TICK_TOUR);
    alphaRad += alphaTmp - alphaTmp*alphaTmp*alphaTmp/3;*/
    AD_alphaTotal = alphaRad*((double)TICK_TOUR)/(2.0*3.1415926535897932384626433832795);
    /*AD_alphaTotal += dAlpha; // Alpha = Alpha + */
    AD_alphaTotal %= (long int) TICK_TOUR;
    if (AD_alphaTotal > TICK_TOUR / 2) // mise en -180;180
    {
        AD_alphaTotal = AD_alphaTotal - TICK_TOUR;

    }
    else if (AD_alphaTotal < -(TICK_TOUR / 2))
    {
        AD_alphaTotal = TICK_TOUR + AD_alphaTotal;
    }

    //alphad = (double)AD_alphaTotal*(2. * 3.1415926535897932384626433832795) / ((double) TICK_TOUR);

    //dX = cos(alphad)*dDelta;
    //dY = sin(alphad)*dDelta;
    dX = cos(alphaRad)*dDelta;
    dY = sin(alphaRad)*dDelta;
#endif


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
	if (AD_abs(commande) > (AD_abs(prec) + acceleration_max)) //acceleration
	{
		if (commande > 0) //en avant
			commande = prec + acceleration_max;
		else //en arriere
			commande = prec - acceleration_max;
	}		
	else if (AD_abs(commande) < ((long int)AD_abs(prec) - (long int)COEFF*(long int)acceleration_max)) //decceleration
	{
                //allumer_del();
		if (commande > 0) //en avant
			commande = prec - (long int)COEFF*(long int)acceleration_max;
		else //en arriere
			commande = prec + (long int)COEFF*(long int)acceleration_max;
	}
				
	//Il ne faut pas oublier de forcer le max speed dans l'asser m�me si set_speed le fait d�ja ( augmente le temps de d�cleration )
	if(commande > AD_MAX_SPEED_REEL) commande = AD_MAX_SPEED_REEL;
	else if ( commande < -AD_MAX_SPEED_REEL ) commande = -AD_MAX_SPEED_REEL;

        if( 0< commande&& commande < AD_MIN_SPEED) commande = AD_MIN_SPEED;
        else if ( 0> commande && commande > -AD_MIN_SPEED ) commande = -AD_MIN_SPEED;
	return commande;
}

void AD_motors_set_speed(short speedL, short speedR)
{
	short commandeL = speedL;
	short commandeR = speedR;
	if ( speedL > 0)
	{
		if (speedL > MAX_SPEED )
		{
			commandeL = MAX_SPEED;   
		}
	}
    else if(speedL < 0)
	{
	  if (-speedL > MAX_SPEED )
	  {
		 	commandeL = -MAX_SPEED;   
	  }
	}
	
	if ( speedR > 0)
	{
                if (speedR > MAX_SPEED )
		{
                    commandeR = MAX_SPEED;
		}
	 } 
    else if(speedR < 0)
	{
	  if (-speedR > MAX_SPEED )
	  {
		 	commandeR = -MAX_SPEED;   
	  }
	}
	motors_set_speed (commandeL,commandeR);
}	


long int echelle(long int c)
{

    return (long int)((MAX_SPEED*1.0/AD_MAX_SPEED)*c);
}

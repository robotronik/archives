/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/
#include "main.h"
#include "myMath.h"
#include "motors.h"
#include "message.h"
#include "AlphaDelta.h"
#include "callage.h"

#ifdef SIMU
#include "prog2.h"
#endif


#define ACCELERATION 1

extern int isPaused;
extern unsigned char commandeSaved;
extern unsigned short consigneXSaved, consigneYSaved;


volatile boolean doitAttendre = TRUE;			// Echantilloneur


//////////////////////////////
#define TICK_ROUE 1440 //Nombre de cran/tick par tour de roue

double AD_consAlpha =   -3.141592/2; //Angle en rad
double AD_consDelta = 0;//1000.0*1440.0/127.0; //Delta : Distance en mm * Distance parcourue par un tour de roue -> en tick 
double AD_ecart;
double AD_kP_delta = 1.0;
double AD_kD_delta = 0.5;

double AD_kP_alpha = 1500.0;
double AD_kD_alpha = 800.0;


long int AD_distG;
long int AD_distD;

double AD_presDelta =  20.0*1440.0/127.0;// 10mm
double AD_presAlpha =0.5*3.141592/180.0 ;
double AD_alpha;
double AD_delta;
double AD_commandeDelta;
double AD_commandePrec = 0;
double AD_commandeAlpha;

char reset = 0;


//////////////////////////////

#ifdef DATA
short cpt_boucle = 0;	// debug : compteur pour limiter les envois de données
#endif


double AD_abs(double valeur)
{
	if(valeur < 0) return -valeur;
	else return valeur;
}

void maj_erreurs( long int erreurs[4],long int erreur)
{
  static short int i = 0;
  static short int j =0;//j à t'on mis erreurs_delta et erreurs_alpha à jour
   
  erreurs[i] = erreur;
  if (j == 1)
  {
  	i++;
  	j=0;
  }
  if(i == 3) i = 0;
  j++;
}

long int deriv_erreurs(long int erreurs[4])
{
  long int moyenne=0;
  
  moyenne = (erreurs[0]-erreurs[1]);

  return moyenne;
}

long int rampe_delta(long int vcommande, long int prec)
{
	long int commande = vcommande;
	if ( commande > 0)
	  {
	  	if ((commande - prec)/2 > ACCELERATION)
	    {
	      commande=prec+ACCELERATION;
	    }

	  if ( commande < MIN_SPEED )
	    {
	      commande = MIN_SPEED;
	    }
	    if (commande > MAX_SPEED )
	    {
		 	commande = MAX_SPEED;   
		}
	   }
	   //Pas de rampe de freinage 
    else if(commande < 0)
	{
	 /* if ( -(commande - AD_commandePrec)/2 > ACCELERATION)
	  {
	      commande=commandePrec-ACCELERATION;
	  }*/
	  if ( -commande < MIN_SPEED )
	  {
	       commande = -MIN_SPEED;
	  }
	  if (-commande > MAX_SPEED )
	  {
		 	commande = -MAX_SPEED;   
	  }
	}
	return commande;
}

long int rampe_alpha(double vcommande)
{
	long int commande=vcommande;
	if ( vcommande > 0)
	{
	  if ( vcommande < MIN_SPEED )
	  {
	      commande = MIN_SPEED;
	  }
		if (vcommande > MAX_SPEED )
		{
			commande = MAX_SPEED;   
		}
	 }
	   //Pas de rampe de freinage 
    else if(vcommande < 0)
	{
	  if ( -vcommande < MIN_SPEED )
	  {
	       commande = -MIN_SPEED;
	  }
	  if (-vcommande > MAX_SPEED )
	  {
		 	commande = MAX_SPEED;   
	  }
	}
	return commande;
}


void asser() {

	long int erreurs_delta[4] = {0,0,0,0};
	long int erreurs_alpha[4] = {0,0,0,0};
	int i ;
	long int debuga = 0;
	///////////////////////////
	motors_get_qei(&AD_distG, &AD_distD);
	/////////////////////////////////
	while(1)
 	{
	 	if( reset)
	 	{
			reset = 0;
			for( i=0; i<4;i++)
			{
				erreurs_delta[i] = 0;
				erreurs_alpha[i] = 0;	
			}	
			AD_delta =0;
			AD_alpha =0;
			AD_commandePrec = 0;
			AD_distG = 0;
			AD_distD = 0;
			motors_reset_qei();
		}
		#ifndef SIMU
		while (doitAttendre);			// Attend l'interruption du timer pour une fréquence régulière
		doitAttendre = TRUE;
		#endif
		AD_commandePrec = AD_commandeDelta;
		///////////////////////
		motors_get_qei(&AD_distG, &AD_distD);
		
		
		AD_alpha = (AD_distD - AD_distG)/(2.0*1831); //1900 = Distance inter roues codeuses en tick/2
		AD_delta = (AD_distD + AD_distG)/2;
		

		//alpha
		AD_ecart = AD_consAlpha - AD_alpha;
		maj_erreurs(erreurs_alpha, AD_ecart);
		AD_commandeAlpha = AD_ecart*AD_kP_alpha  - deriv_erreurs(erreurs_alpha)*AD_kD_alpha;	
		if(AD_abs(AD_ecart) < AD_presAlpha)
		{
			AD_commandeAlpha=0;	
		}
		//delta
		AD_ecart = AD_consDelta - AD_delta;
		maj_erreurs(erreurs_delta, AD_ecart);
		debuga = deriv_erreurs(erreurs_delta);
		AD_commandeDelta = AD_ecart*AD_kP_delta - deriv_erreurs(erreurs_delta)*AD_kD_delta;

		//Rampe de vitesse
		AD_commandeAlpha = rampe_alpha(AD_commandeAlpha);
		AD_commandeDelta = rampe_delta(AD_commandeDelta, AD_commandePrec);
		//Precision en delta
		if( AD_abs(AD_ecart) < AD_presDelta )
		{
			AD_commandeDelta = 0;
		}
		
		
			motors_set_speed (AD_commandeDelta  - AD_commandeAlpha, AD_commandeDelta + AD_commandeAlpha);
		   

//////////////////////////////////

		if (!doitAttendre) allumer_del();	// Timer trop rapide : allume LED debug
	}
}

#ifndef SIMU
void __attribute__((interrupt, auto_psv)) _MPWM1Interrupt(void) {
	IFS3bits.PWM1IF = 0;	// On s'acquitte de l'interruption
	doitAttendre = FALSE;
}
#endif

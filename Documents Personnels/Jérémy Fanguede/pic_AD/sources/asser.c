/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/
#include "main.h"
#include "myMath.h"
#include "motors.h"
#include "message.h"
#include "XYAngle.h"
#include "AlphaDelta.h"
#include "callage.h"

#ifdef SIMU
#include "prog2.h"
#endif


#define ACCELERATION 10

extern int isPaused;
extern unsigned char commandeSaved;
extern unsigned short consigneXSaved, consigneYSaved;


volatile boolean doitAttendre = TRUE;			// Echantilloneur


//////////////////////////////
#define TICK_ROUE 1440
char type = 1; // 1 Mode Alpha - 0 Mode Delta

double AD_consAlpha =-3.14/2.0; //Angle 
double AD_ecart;
long double AD_kP = 100;
long double AD_kD = 10.0;
long int AD_distG;
long int AD_distD;

double AD_alpha;
long int AD_commande;
long int AD_commandePrec = 0;
long int AD_commandeAlpha;



//////////////////////////////

#ifdef DATA
short cpt_boucle = 0;	// debug : compteur pour limiter les envois de données
#endif

void maj_erreur( double erreurs[4], erreur)
{
  static char i = 0;
  erreurs[i] = erreur;
  if(i>3) i = 0;
}

double moyenne_erreurs( double erreurs)
{
  int i = 0;
  double moyenne;
  for (i = 0; i < 4; i++)
  {
    moyenne += erreurs[i];
  }
  return moyenne/3
}


void asser() {

  double erreurs[4] = {0,0,0,0};
  ///////////////////////////
  motors_get_qei(&AD_distG, &AD_distD);
  AD_distGPrec = AD_distG;
  AD_distDPrec = AD_distD;
  /////////////////////////////////
  while(1)
    {
#ifndef SIMU
      while (doitAttendre);			// Attend l'interruption du timer pour une fréquence régulière
      doitAttendre = TRUE;
#endif
      AD_commandePrec = AD_commande;
      ///////////////////////
      motors_get_qei(&AD_distG, &AD_distD);

		
      AD_alpha = (AD_distD - AD_distG)/(2.0*1900);  
      AD_delta = (AD_distD + AD_distG)/2;
		
      if(type == 1)
      {
	  AD_ecart = AD_consAlpha - AD_alpha;
	  maj_erreurs(erreurs, AD_ecart);
	  AD_commande = AD_ecart*AD_kP + moyenne_erreurs(erreurs)*AD_kD;	
      }
      else
      {
	  AD_ecart = AD_consDelta - AD_delta;
	  maj_erreurs(erreurs, AD_ecart);
	  AD_commande = AD_ecart*AD_kP + moyenne_erreurs(erreurs)*AD_kD;
      }
      //Rampe de vitesse

      if ( AD_commande > 0)
	{
	  if ( (AD_commande - AD_commandePrec)/2 > ACCELERATION)
	    {
	      AD_commande=AD_commandePrec+ACCELERATION;
	    }

	  if ( AD_commande < MIN_SPEED )
	    {
	      AD_commande = MIN_SPEED;
	    }
	}
      else if(AD_commande < 0)
	{
	  if ( -(AD_commande - AD_commandePrec)/2 > ACCELERATION)
	    {
	      AD_commande=AD_commandePrec-ACCELERATION;
	    }
	  if ( -AD_commande < MIN_SPEED )
	    {
	      AD_commande = -MIN_SPEED;
	    }
	}
		
		
      motors_set_speed (-AD_commande, AD_commande);
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

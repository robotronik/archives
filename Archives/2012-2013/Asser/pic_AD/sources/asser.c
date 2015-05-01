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


extern int isPaused;
extern unsigned char commandeSaved;
extern unsigned short consigneXSaved, consigneYSaved;


volatile boolean doitAttendre = TRUE;			// Echantilloneur


//////////////////////////////

long int AD_consAlpha = 0;
long int AD_consDelta =  0;//1000*MM_TO_TICKS;//0;//1000*MM_TO_TICKS;//1000.0*10950/1000; //Delta : 10950 nombre de tick pour 1 metre valeur expermimental
long int AD_ecartDelta;
long int AD_ecartAlpha;
volatile AD_K kP_delta = { 2, 4 };//{ 1., 1800. };//{ 0.09, 1800. };//{1.0, 20000.};//{0., 0.};//{1.0, 10000.};//{ 1.0, 17000.};
volatile AD_K kD_delta = { 2, 0 };//{ 0., 0. };//{ 0.18, 1800. };//{2.0, 20000.0};//{0., 0.};//{2.0, 0.0};//{ 0., 20000.};

volatile AD_K kP_alpha = { 3, 2};//{ 0.09, 225.}; //{ 1., 2500.};
volatile AD_K kD_alpha = { 0 , 0 };//{ 0. , 243. }; //{ 0., 2700.};

long int AD_distG; //en tick
long int AD_distD;
long int AD_precG; //en tick
long int AD_precD;


long int AD_dG; //en tick
long int AD_dD;

long int AD_presDelta =  100;//1.*10950./1000.;// 5mm // en mm
long int AD_presAlpha = 0.5*TICK_TOUR/360; // en degrés


long int AD_alpha;
long int AD_delta;
long int AD_commandeDelta;
long int AD_commandePrecDelta = 0;
long int AD_commandePrecAlpha = 0;
long int AD_commandeAlpha;
long int AD_alphaTotal = 0; //en ticks

volatile long posX = 0;				
volatile long posY = 0;	

volatile char reset = 0;

char done=1;


//////////////////////////////

#ifdef DATA
short cpt_boucle = 0;	// debug : compteur pour limiter les envois de données
#endif

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
	 
		#ifndef SIMU
		while (doitAttendre);			// Attend l'interruption du timer pour une fréquence régulière
		doitAttendre = TRUE;
		#endif
		AD_commandePrecDelta = AD_commandeDelta;
		AD_commandePrecAlpha = AD_commandeAlpha;
		if( reset)
	 	{
		 	majAlpha();
			reset = 0;
			for( i=0; i<4;i++)
			{
				erreurs_delta[i] = 0;
				erreurs_alpha[i] = 0;	
			}	
			AD_delta =0;
			AD_alpha =0;
			AD_commandePrecDelta = 0;
			AD_commandePrecAlpha =0;
			AD_distG = 0;
			AD_distD = 0;
			done =0;
			motors_reset_qei();
		}
		///////////////////////
		
		
 		AD_precG = AD_distG;
		AD_precD = AD_distD;
		motors_get_qei(&AD_distG, &AD_distD);

		//Calcul des variations
		AD_dG = AD_distG-AD_precG;
		AD_dD = AD_distD-AD_precD;
		
		AD_alpha += (AD_dD - AD_dG);
		
		AD_delta += (AD_dD + AD_dG)/2;
		majXYAngle(AD_alphaTotal + AD_alpha, AD_delta );

		//alpha
		AD_ecartAlpha = AD_consAlpha - AD_alpha;
		maj_erreurs(erreurs_alpha, AD_ecartAlpha);
		if (AD_consAlpha == 0) //Delta pur
			AD_commandeAlpha = AD_ecartAlpha*kP_delta.alpha  + deriv_erreurs(erreurs_alpha)*kD_delta.alpha;	
		else
			AD_commandeAlpha = AD_ecartAlpha*kP_alpha.alpha  + deriv_erreurs(erreurs_alpha)*kD_alpha.alpha;

		//delta
		AD_ecartDelta = AD_consDelta - AD_delta;
		maj_erreurs(erreurs_delta, AD_ecartDelta);
		debuga = deriv_erreurs(erreurs_delta);
		if (AD_consAlpha == 0) //Deltapur
			AD_commandeDelta = AD_ecartDelta*kP_delta.delta + deriv_erreurs(erreurs_delta)*kD_delta.delta;
		else // alpha pur 
			AD_commandeDelta = AD_ecartDelta*kP_alpha.delta + deriv_erreurs(erreurs_delta)*kD_alpha.delta;

		//Rampe de vitesse
		if (AD_consAlpha == 0) //Delta pur
			AD_commandeDelta = rampe(AD_commandeDelta, AD_commandePrecDelta);
		else
			AD_commandeAlpha = rampe(AD_commandeAlpha, AD_commandePrecAlpha);
		
		if ( (AD_abs(AD_ecartAlpha) < AD_presAlpha) && (AD_abs(AD_ecartDelta) < AD_presDelta) ) //On test si on est au bonne endroit 
		{
			if(((AD_max((AD_commandeDelta  - AD_commandeAlpha),(AD_commandeDelta + AD_commandeAlpha)) < VIT_MAX) &&( AD_consAlpha == 0) ) || (AD_consDelta == 0)) // Evite que le robot tombe en arrière
			{
				AD_commandeAlpha=0;	
				AD_commandeDelta=0;
				if(!done)
				sendDone();
				done = 1;
			}
			
		}
		
		
		AD_motors_set_speed((short)(AD_commandeDelta  - AD_commandeAlpha),(short)(AD_commandeDelta + AD_commandeAlpha));
		   

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

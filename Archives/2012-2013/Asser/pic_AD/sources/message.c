/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "asser.h"
#include "motors.h"
#include "uart.h"

extern volatile int isPSoCReady;

#define X_START			LONG_ARRIERE
#define Y_START			250	// [mm]
/*
unsigned char commande[NB_MAX_COMMANDES];
short nbCommande = 0;
short indexCommande = 0;
int isPaused = 0;
unsigned char commandeSaved;
unsigned short consigneXSaved, consigneYSaved;

void fin(void);
void purpleStart(void);
void redStart(void);
void sendPos(void);
void test_branchement(void);
void set_commande(void);
*/
extern volatile char reset;

//Asser 
extern long int AD_consAlpha ; //Angle en rad
extern long int AD_consDelta; //Delta : Distance en mm * Distance parcourue par un tour de roue -> en tick 
extern long posX;
extern long posY;
extern long int  AD_alphaTotal ;
extern long int AD_alpha;
extern volatile AD_K kP_delta;
extern volatile AD_K kD_delta;
extern volatile AD_K kP_alpha;
extern volatile AD_K kD_alpha;




void message_processing(unsigned char inMessage)
{
	static char nb_recu = 0;
	static int boolConfig = 0;
	static TrameAsser t;
	static TrameConfig tc;
	short valeur;
	Trame32 t32;
	TrameAsser t16;
	

	if(nb_recu == 0)
	{
		t.trShort.Xo = inMessage;
		if (t.trDelta.codet == 0b010 ||t.trDelta.codet == 0b101 ||t.trDelta.codet == 0b000 || 
		    t.trDelta.codet == CODE_CONFIG || t.trDelta.codet == CODE_GET_XY || t.trDelta.codet == CODE_GET_ALPHA )
			nb_recu++;
	}
	else if (nb_recu == 1)
	{
		t.trShort.oX =  inMessage;
		
		switch (t.trDelta.codet)
		{
			case CODE_DELTA : //0b10 : //delta
				if (t.trDelta.signe == 1)
					valeur = - t.trDelta.delta;
				else
					valeur = t.trDelta.delta;
				//AD_consDelta = valeur*10950./1000.;
				AD_consDelta = valeur*MM_TO_TICKS;
				AD_consAlpha = 0;
				nb_recu=0;
				reset = 1;
			break;
			
			case CODE_ALPHA : //0b101 : //alpha
				if (t.trAlpha.signe == 1)
					valeur = - t.trAlpha.alpha;
				else
					valeur = t.trAlpha.alpha;
					
				AD_consDelta = 0;
				//AD_consAlpha = valeur*3.14/180.0;
				AD_consAlpha = valeur*TICK_TOUR/360;
				nb_recu=0;
				reset = 1;
			break;
			
			case CODE_STOP : //0b000 :
				AD_consDelta = 0;
				AD_consAlpha = 0;
				nb_recu=0;
				reset = 1;
			break;
			
			case CODE_CONFIG : //0b111
				boolConfig = 1;
				tc.tr4Char.Xooo = t.trShort.Xo;
				tc.tr4Char.oXoo = t.trShort.oX;
				nb_recu++;
			break;	
			
			case CODE_GET_XY :
				nb_recu = 0;
				t32.tpos.y = posY;
				t32.tpos.x = posX;
				if(posY < 0)
					t32.tpos.y = 0;
				if(posX <0)
					t32.tpos.x = 0;
					
				t32.tpos.codet = CODE_GET_XY;
				send_long(t32.l);
			break;
			
			case CODE_GET_ALPHA :
				nb_recu = 0 ;
				if((AD_alphaTotal +AD_alpha) < 0)
				{
					unsigned long alphaCalc =  - (AD_alphaTotal + AD_alpha);
					alphaCalc %= (long int)TICK_TOUR;
					alphaCalc *= 360;
					alphaCalc /= (unsigned long int)TICK_TOUR;
					t16.trAlpha.alpha = alphaCalc;
					t16.trAlpha.signe = 1;	
				}
				else
				{
					unsigned long alphaCalc =  (AD_alphaTotal + AD_alpha);
					alphaCalc %= (long int)TICK_TOUR;
					alphaCalc *= 360;
					alphaCalc /= (unsigned long int)TICK_TOUR;
					t16.trAlpha.alpha =  alphaCalc;
					t16.trAlpha.signe = 0;
				}
				t16.trAlpha.codet = CODE_GET_ALPHA;
				send_short(t16.us);
			break; 
				
				
				
					
		}
		
		
	}
	else if (nb_recu == 2 )
	{
		tc.tr4Char.ooXo = inMessage;
		nb_recu++;
	}
	else
	{
		tc.tr4Char.oooX = inMessage;
		switch (tc.conf.pid)
		{
			case CODE_P : //0b00
				switch (tc.conf.ad1)
				{
					case CODE_CONFIG_DELTA : //0b0
						switch (tc.conf.ad2)
						{
							case CODE_CONFIG_DELTA : // 0b0
								kP_delta.delta = tc.conf.ent;//(double)(tc.conf.ent + tc.conf.dec/100.);
							break;
							case CODE_CONFIG_ALPHA : // 0b1
								kP_delta.alpha = tc.conf.ent;//(double)(tc.conf.ent + tc.conf.dec/100.);
							break;
						}
					break;
					case CODE_CONFIG_ALPHA : //0b1
						switch (tc.conf.ad2)
						{
							case CODE_CONFIG_DELTA : // 0b0
								kP_alpha.delta = tc.conf.ent;//(double)(tc.conf.ent + tc.conf.dec/100.);
							break;
							case CODE_CONFIG_ALPHA : // 0b1
								kP_alpha.alpha = tc.conf.ent;//(double)(tc.conf.ent + tc.conf.dec/100.);
							break;
						}
					break;
				}		
			break;
			case CODE_D : //0b10
				switch (tc.conf.ad1)
				{
					case CODE_CONFIG_DELTA : //0b0
						switch (tc.conf.ad2)
						{
							case CODE_CONFIG_DELTA : // 0b0
								kD_delta.delta = tc.conf.ent;//(double)(tc.conf.ent + tc.conf.dec/100.);
							break;
							case CODE_CONFIG_ALPHA : // 0b1
								kD_delta.alpha = tc.conf.ent;//(double)(tc.conf.ent + tc.conf.dec/100.);
							break;
						}
					break;
					case CODE_CONFIG_ALPHA : //0b1
						switch (tc.conf.ad2)
						{
							case CODE_CONFIG_DELTA : // 0b0
								kD_alpha.delta = tc.conf.ent;//(double)(tc.conf.ent + tc.conf.dec/100.);
							break;
							case CODE_CONFIG_ALPHA : // 0b1
								kD_alpha.alpha = tc.conf.ent;//(double)(tc.conf.ent + tc.conf.dec/100.);
							break;
						}
					break;
				}		
			break;
		
		}	
		nb_recu = 0;
	}		
	
}

void fin(void) {
	motors_stop();
	while (1) {
		allumer_del();
		pause_ms(500);
		eteindre_del();
		pause_ms(500);
	}
}
/*
void test_branchement(void) {
	//Test de commande min et de sens de rotation : tourne vers la gauche, revient
	long posL1, posR1, posL2, posR2;
	short diffL, diffR;
	
	motors_get_qei(&posL1, &posR1);
	motors_set_speed(0,MIN_SPEED);
	allumer_del();
	pause_s(5);
	motors_get_qei(&posL2, &posR2);
	diffL = posL2 - posL1;
	diffR = posR2 - posR1;
	if ((diffL > (METRE/200)) || (diffR < (METRE/200))) UART_putc(TEST_BRANCHEMENT);
	motors_set_speed(0, -MIN_SPEED);
	eteindre_del();
	do {
		motors_get_qei(&posL2, &posR2);
		diffL = posL2 - posL1;
		diffR = posR2 - posR1;
	} while (diffL > METRE/500 || diffL < -METRE/500 || diffR > METRE/500 || diffR < -METRE/500);
	
	motors_get_qei(&posL1, &posR1);
	motors_set_speed(MIN_SPEED, 0);
	allumer_del();
	pause_s(5);
	motors_get_qei(&posL2, &posR2);
	diffL = posL2 - posL1;
	diffR = posR2 - posR1;
	if (diffR > METRE/200 || diffL < METRE/200) UART_putc(TEST_BRANCHEMENT);
	motors_set_speed(-MIN_SPEED, 0);
	eteindre_del();
	do {
		motors_get_qei(&posL2, &posR2);
		diffL = posL2 - posL1;
		diffR = posR2 - posR1;
	} while (diffL > METRE/500 || diffL < -METRE/500 || diffR > METRE/500 || diffR < -METRE/500);
	
	motors_stop();
	UART_putc(DONE);
}

*/

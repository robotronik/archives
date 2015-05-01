/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "asser.h"
#include "motors.h"

extern volatile int isPSoCReady;

#define X_START			LONG_ARRIERE
#define Y_START			250	// [mm]

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

extern char reset;

//Asser 
extern double AD_consAlpha ; //Angle en rad
extern double AD_consDelta; //Delta : Distance en mm * Distance parcourue par un tour de roue -> en tick 

void message_processing(unsigned char inMessage)
{
	/*static char nb_recu = 0;
	static TrameAsser t;
	 
	
	if(nb_recu == 0)
	{
		t.trShort.Xo = inMessage;
		if (t.trDelta.codet == 0b010 ||t.trDelta.codet == 0b101 ||t.trDelta.codet == 0b000)
			nb_recu++;
	}
	else
	{
		t.trShort.oX =  inMessage;
		
		
		reset = 1;
		switch (t.trDelta.codet)
		{
			case 0b10 : //delta
				AD_consDelta = t.trDelta.delta*1440.0/127.0;
				AD_consAlpha = 0;
				
			break;
			case 0b101 :
				AD_consDelta = 0;
				AD_consAlpha = t.trAlpha.alpha*3.14/180.0;
			break;
			
			case 0b000 :
				AD_consDelta = 0;
				AD_consAlpha = 0;
			break;
					
		}
		
		nb_recu=0;
	}
	*/
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


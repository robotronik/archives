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


void message_processing(unsigned char inMessage)
{
	static short indexLibreCommande = 0;
	static short nbArgRestant = 0;
	static unsigned char commandePrec;
	static unsigned char argPrec;
	
	if (!(isPSoCReady || inMessage == ACK || inMessage == TEST)) return;
	
	if (nbArgRestant) {
		nbArgRestant --;
		switch (commandePrec) {
			case SUPPR:
				if (inMessage > nbCommande) inMessage = nbCommande;
				short i, indexSuppr = indexCommande;
				// Recherche de l'index inferieur de suppression
				for (i=0; i<nbCommande - inMessage; i++)
					switch (commande[indexSuppr]) {
					case XY_RAPIDE:
					case XY_COURBE:
						indexSuppr += 9; indexSuppr %= NB_MAX_COMMANDES;
						break;
					default:	// Alpha
						indexSuppr += 4; indexSuppr %= NB_MAX_COMMANDES;
					}
				// Suppression
				indexLibreCommande = indexSuppr;
				nbCommande -= inMessage;
				if (!nbCommande) {
					isPaused = 0;
				}	
				break;

				commande[indexLibreCommande] = inMessage;
				indexLibreCommande ++; indexLibreCommande %= NB_MAX_COMMANDES;
				if (!nbArgRestant) nbCommande ++;
		}
		argPrec = inMessage;
	} else {
		switch(inMessage)
		{
			case XY_RAPIDE:
			case XY_COURBE:
				commande[indexLibreCommande] = inMessage;
				indexLibreCommande ++; indexLibreCommande %= NB_MAX_COMMANDES;
				nbArgRestant = 8;
				break;
			case CALLE_G:
			case CALLE_H:
			case CALLE_D:
				commande[indexLibreCommande] = inMessage;
				indexLibreCommande ++; indexLibreCommande %= NB_MAX_COMMANDES;
				nbCommande ++;
				break;
			case SUPPR: nbArgRestant = 1; break;
			case FIN_MATCH: fin(); break;
			case PURPLE: purpleStart(); break;
			case RED: redStart(); break;
			case TEST:
				if (isPSoCReady) break;
				isPSoCReady = TRUE;
				UART_putc(ACK);
				break;
			case ACK:
				isPSoCReady = TRUE;
				break;
			case POS: sendPos(); break;



			case TEST_BRANCHEMENT:
				test_branchement();
				break;
			default:	// Alpha
				commande[indexLibreCommande] = inMessage;
				indexLibreCommande ++; indexLibreCommande %= NB_MAX_COMMANDES;
				nbArgRestant = 3;
		}
		commandePrec = inMessage;
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

void redStart(void) {


}

void purpleStart(void) {


}

void sendPos(void) {

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


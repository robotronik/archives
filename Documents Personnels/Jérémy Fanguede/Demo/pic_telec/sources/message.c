/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/

#include "main.h"
#include "message.h"
#include "asser.h"
#include "motors.h"

extern unsigned char toDo;
extern unsigned char recu;

extern unsigned short coef_frein;
extern unsigned short coef_frein_alpha;
extern unsigned short coef_frein_alpha2;
extern unsigned short min_speed;
extern unsigned char commandeCourante;
extern unsigned short consigneX, consigneY; 	// Consigne (X, Y) [0xFFFF = 3 m]
extern unsigned long consPrecision;
extern unsigned short consAlpha;				// [TOURieme de tour]
extern volatile int isPSoCReady;

#define LONG_ARRIERE	36	// [mm]
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
	
	
	if (inMessage == 'a')
	{
		toDo ='a';
		recu =1;
		return;
	}	
	else if (inMessage == 'r')
	{
		toDo = 'r';
		recu =1;
		return;
	}
	else if (inMessage == 'g')
	{
		toDo = 'g';
		recu =1;
		return;
	}
	else if (inMessage == 'd')
	{
		toDo = 'd';
		recu =1;
		return;
	}
	else if (inMessage == 's')
	{
		toDo = 's';
		recu =1;
		return;
	}	
	

	
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
					commandeCourante = FREINAGE;
				}	
				break;
			case SET_COEF_FREIN:
				if (!nbArgRestant) coef_frein = ((unsigned short) argPrec << 8) | inMessage; break;
			case SET_COEF_FREIN_ALPHA:
				if (!nbArgRestant) coef_frein_alpha = ((unsigned short) argPrec << 8) | inMessage; break;
			case SET_COEF_FREIN_ALPHA2:
				if (!nbArgRestant) coef_frein_alpha2 = ((unsigned short) argPrec << 8) | inMessage; break;
			case SET_MIN_SPEED:
				if (!nbArgRestant) min_speed = ((unsigned short) argPrec << 8) | inMessage; break;
			default:
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
			case SET_COEF_FREIN: nbArgRestant = 2; break;
			case SET_COEF_FREIN_ALPHA: nbArgRestant = 2; break;
			case SET_COEF_FREIN_ALPHA2: nbArgRestant = 2; break;
			case SET_MIN_SPEED: nbArgRestant = 2; break;
			case GET_COEF_FREIN:
				UART_putc(GET_COEF_FREIN);
				UART_putc(coef_frein >> 8);
				UART_putc(coef_frein);
				break;
			case GET_COEF_FREIN_ALPHA:
				UART_putc(GET_COEF_FREIN_ALPHA);
				UART_putc(coef_frein_alpha >> 8);
				UART_putc(coef_frein_alpha);
				break;
			case GET_COEF_FREIN_ALPHA2:
				UART_putc(GET_COEF_FREIN_ALPHA2);
				UART_putc(coef_frein_alpha2 >> 8);
				UART_putc(coef_frein_alpha2);
				break;
			case GET_MIN_SPEED:
				UART_putc(GET_MIN_SPEED);
				UART_putc(min_speed >> 8);
				UART_putc(min_speed);
				break;
			case PAUSE:
				if (!isPaused) {	// Début de pause
					commandeSaved = commandeCourante;
					consigneXSaved = consigneX;
					consigneYSaved = consigneY;
					commandeCourante = FREINAGE;
				}
				isPaused ++;
				break;
			case GET_COMMANDE:
				set_commande();
				break;
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

void fin(void) 
{
	motors_stop();
	while (1) {
		allumer_del();
		pause_ms(500);
		eteindre_del();
		pause_ms(500);
	}
}

void redStart(void) 
{
	posX.all = (long long) 0xFFFFFFFF*X_START / 3000;
	posY.all = (long long) 0xFFFFFFFF*Y_START / 3000;
	alphaInit = TOUR >> 1;
	posAlphaPrec = alphaInit;
	
	/*unsigned long tmp;
	message_processing(CALLE_H);
	
	message_processing(XY_RAPIDE);
	tmp = (long)0xFFFF*4/30;
	message_processing(tmp >> 8);
	message_processing(tmp);
	tmp = (long)0xFFFF*25/300;
	message_processing(tmp >> 8);
	message_processing(tmp);
	tmp = (long)0xFFFF*2/3000;
	tmp *= tmp;
	message_processing(tmp >> 24);
	message_processing(tmp >> 16);
	message_processing(tmp >> 8);
	message_processing(tmp);
	
	message_processing(0);
	message_processing(0);
	message_processing(0);
	message_processing(10);
	message_processing(CALLE_G);
	
	message_processing(XY_RAPIDE);
	tmp = (long)0xFFFF*25/300;
	message_processing(tmp >> 8);
	message_processing(tmp);
	tmp = (long)0xFFFF*25/300;
	message_processing(tmp >> 8);
	message_processing(tmp);
	tmp = (long)0xFFFF*2/3000;
	tmp *= tmp;
	message_processing(tmp >> 24);
	message_processing(tmp >> 16);
	message_processing(tmp >> 8);
	message_processing(tmp);
	
	message_processing(0);
	message_processing(0);
	message_processing(0);
	message_processing(10);*/
	
	// posAlphaInit = 0;
}

void purpleStart(void) {
	posX.all = (long long) 0xFFFFFFFF*(3000 - X_START) / 3000;
	posY.all = (long long) 0xFFFFFFFF*Y_START / 3000;
	alphaInit = 0;
	posAlphaPrec = alphaInit;
	
	/*unsigned long tmp;
	message_processing(CALLE_H);
	
	message_processing(XY_RAPIDE);
	tmp = (long)0xFFFF*26/30;
	message_processing(tmp >> 8);
	message_processing(tmp);
	tmp = (long)0xFFFF*25/300;
	message_processing(tmp >> 8);
	message_processing(tmp);
	tmp = (long)0xFFFF*2/3000;
	tmp *= tmp;
	message_processing(tmp >> 24);
	message_processing(tmp >> 16);
	message_processing(tmp >> 8);
	message_processing(tmp);
	
	message_processing(TOUR >> 9);
	message_processing((unsigned char) (TOUR >> 1));
	message_processing(0);
	message_processing(10);
	
	message_processing(CALLE_D);
	
	message_processing(XY_RAPIDE);
	tmp = (long)0xFFFF*275/300;
	message_processing(tmp >> 8);
	message_processing(tmp);
	tmp = (long)0xFFFF*25/300;
	message_processing(tmp >> 8);
	message_processing(tmp);
	tmp = (long)0xFFFF*2/3000;
	tmp *= tmp;
	message_processing(tmp >> 24);
	message_processing(tmp >> 16);
	message_processing(tmp >> 8);
	message_processing(tmp);
	
	message_processing(TOUR >> 9);
	message_processing((unsigned char) (TOUR >> 1));
	message_processing(0);
	message_processing(10);*/
	
	// posAlphaInit = TOUR >> 1;
}

void sendPos(void) {
	UART_putc(POS);
	UART_putc(posX.part.high >> 8);
	UART_putc(posX.part.high);
	UART_putc(posY.part.high >> 8);
	UART_putc(posY.part.high);
	UART_putc(posAlpha >> 8);
	UART_putc(posAlpha);
}

void test_branchement(void) {
	//Test de commande min et de sens de rotation : tourne vers la gauche, revient
	long posL1, posR1, posL2, posR2;
	short diffL, diffR;
	
	motors_get_qei(&posL1, &posR1);
	motors_set_speed(0, min_speed);
	allumer_del();
	pause_s(5);
	motors_get_qei(&posL2, &posR2);
	diffL = posL2 - posL1;
	diffR = posR2 - posR1;
	if ((diffL > (METRE/200)) || (diffR < (METRE/200))) UART_putc(TEST_BRANCHEMENT);
	motors_set_speed(0, -min_speed);
	eteindre_del();
	do {
		motors_get_qei(&posL2, &posR2);
		diffL = posL2 - posL1;
		diffR = posR2 - posR1;
	} while (diffL > METRE/500 || diffL < -METRE/500 || diffR > METRE/500 || diffR < -METRE/500);
	
	motors_get_qei(&posL1, &posR1);
	motors_set_speed(min_speed, 0);
	allumer_del();
	pause_s(5);
	motors_get_qei(&posL2, &posR2);
	diffL = posL2 - posL1;
	diffR = posR2 - posR1;
	if (diffR > METRE/200 || diffL < METRE/200) UART_putc(TEST_BRANCHEMENT);
	motors_set_speed(-min_speed, 0);
	eteindre_del();
	do {
		motors_get_qei(&posL2, &posR2);
		diffL = posL2 - posL1;
		diffR = posR2 - posR1;
	} while (diffL > METRE/500 || diffL < -METRE/500 || diffR > METRE/500 || diffR < -METRE/500);
	
	motors_stop();
	UART_putc(DONE);
}

void set_commande(void) {
	UART_putc(GET_COMMANDE);
	unsigned char commande;
	unsigned short consX, consY;
	if (isPaused) {
		commande = commandeSaved;
		consX = consigneXSaved;
		consY = consigneYSaved;
	} else {
		commande = commandeCourante;
		consX = consigneX;
		consY = consigneY;
	}	
	if (commande < MAX_ALPHA) {
		UART_putc(consAlpha >> 8);
		UART_putc(consAlpha);
		UART_putc(consPrecision >> 8);
		UART_putc(consPrecision);
	} else {
		UART_putc(commande);
		if (commande == XY_RAPIDE || commande == XY_COURBE) {
			UART_putc(consX >> 8);
			UART_putc(consX);
			UART_putc(consY >> 8);
			UART_putc(consY);
			UART_putc(consPrecision >> 24);
			UART_putc(consPrecision >> 16);
			UART_putc(consPrecision >> 8);
			UART_putc(consPrecision);
		}	
	}
}

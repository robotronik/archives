/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/




#include "main.h"
#include "message.h"
#include "asser.h"
#include "motors.h"

#include <stdio.h>

unsigned char commande[NB_MAX_COMMANDES];
int nbCommande = 0;
int indexCommande = 0;

int couleurRobot = PURPLE;

static void fin(void);
static void redStart(void);
static void purpleStart(void);
static void test_uart(void);
static void sendPos(void);

void message_processing(unsigned char inMessage)
{
   // printf("Commande recue : %u\n", inMessage);
	static int indexLibreCommande = 0;
	static int nbArgRestant = 0;
	static boolean waitArgSuppr = FALSE;

	if (waitArgSuppr) {
		int i;
		for (i=0; i<nbCommande - inMessage; i++)
			switch (commande[indexCommande]) {
			case XY_RAPIDE:
			case XY_COURBE:
				indexCommande += 7; indexCommande %= NB_MAX_COMMANDES;
				break;
			default:
				indexCommande += 5; indexCommande %= NB_MAX_COMMANDES;
			}
		nbCommande -= inMessage;
	} else if (nbArgRestant) {
		nbArgRestant --;
		commande[indexLibreCommande] = inMessage;
		indexLibreCommande ++; indexLibreCommande %= NB_MAX_COMMANDES;
		if (!nbArgRestant) nbCommande ++;
	} else
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
			case SUPPR: waitArgSuppr = TRUE; break;
			case FIN_MATCH: fin(); break;
			case RED: redStart(); break;
			case PURPLE: purpleStart(); break;
			case TEST: test_uart(); break;
			case GET_POS: sendPos(); break;
			default:
				commande[indexLibreCommande] = inMessage;
				indexLibreCommande ++; indexLibreCommande %= NB_MAX_COMMANDES;
				nbArgRestant = 2;
		}
}

static void fin(void) {
	motors_stop();
	while (1) {
		allumer_del();
		pause_ms(500);
		eteindre_del();
		pause_ms(500);
	}
}

static void redStart(void) {
	posX.all = (long long) 0xFFFFFFFF * RED_START_X / 3000;
	posY.all = (long long) 0xFFFFFFFF * RED_START_Y / 3000;
	alphaInit = RED_START_ALPHA;
	couleurRobot = RED;

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

static void purpleStart(void) {
	posX.all = (long long) 0xFFFFFFFF * PURPLE_START_X / 3000;
	posY.all = (long long) 0xFFFFFFFF * PURPLE_START_Y / 3000;
	alphaInit = PURPLE_START_ALPHA;
	couleurRobot = PURPLE;

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

static void test_uart(void) {
	allumer_del();
	UART_putc(TEST);
	pause_ms(500);
	eteindre_del();
}

static void sendPos(void) {
	UART_putc(POS);
	UART_putc(posX.part.high >> 8);
	UART_putc(posX.part.high);
	UART_putc(posY.part.high >> 8);
	UART_putc(posY.part.high);
	UART_putc(posAlpha >> 8);
	UART_putc(posAlpha);
}


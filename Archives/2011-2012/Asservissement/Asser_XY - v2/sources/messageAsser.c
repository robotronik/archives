/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/

#include "Boolean.h"
#include "messageAsser.h"
#include "motors.h"
#include "debug.h"
#include "XYAngle.h"
#include "QEI.h"
#include "asser.h"

unsigned char commande[NB_MAX_OCT_COMMANDES];
short nbCommande = 0;
short indexCommande = 0;
short indexLibreCommande = 0;
boolean isPSoCReady = FALSE;
boolean isPaused = 0;

inline void suppr(unsigned char nbSuppr);
inline void test_branchement(void);
inline void giveCommande(void);

inline void connectPSoC(void) {
	UART_putc(START);
	while (!isPSoCReady) {
		messages_processing();
		UART_putc(CONNECT);
		PAUSE_MS(10);
	}
}

inline void messages_processing(void)
{
	// Alloué dès la compil
	static unsigned char inByte;
	
	// Mémorisation utile
	static short nbArgRestant = 0;
	static unsigned char commandePrec;
	static unsigned char argPrec[8];	// 8 arguments max
	
	/*if (!doitAttendre) allumer_del();	// Timer trop rapide
	else eteindre_del();*/
	
	// Rend la main à l'asser si pas assez de temps
#if defined(COLLECTEUR) || defined(SIMU)
	while (UART_getc(&inByte)) {
#else	// #if defined(COLLECTEUR) || defined(SIMU)
	while (doitAttendre && UART_getc(&inByte)) {
#endif	// #if defined(COLLECTEUR) || defined(SIMU)
		if (!(isPSoCReady || inByte == ACK_CONNECT || inByte == CONNECT)) continue;
		
		if (!nbArgRestant) {
			nbArgRestant = nbArgPSoCToAsser[inByte];
			switch(inByte)
			{
				case CONNECT:
					UART_putc(ACK_CONNECT);
					break;
				case ACK_CONNECT:
					isPSoCReady = TRUE;
					break;
				case GET_COEF_FREIN:
					UART_putc(GET_COEF_FREIN);
					UART_send_tab((unsigned char *) &coef_frein, 2);
					break;
				case GET_COEF_FREIN_ALPHA:
					UART_putc(GET_COEF_FREIN_ALPHA);
					UART_send_tab((unsigned char *) &coef_frein_alpha, 2);
					break;
				case GET_MIN_SPEED:
					UART_putc(GET_MIN_SPEED);
					UART_send_tab((unsigned char *) &minSpeed, 2);
					break;
				case GET_DEBUG_SETTING:
					UART_putc(GET_DEBUG_SETTING);
					UART_send_tab((unsigned char *) &debug_setting, 4);
					break;
				case GET_ACCEL_MAX:
					UART_putc(GET_ACCEL_MAX);
					UART_putc(accel_max);
					break;
				case PAUSE:
					isPaused = !isPaused;
					break;
				case GET_QEI:
					isDebugEnabled = !isDebugEnabled;
					break;
				case FIN_MATCH:			fin(); break;
				case PURPLE:			purpleStart(); break;
				case RED:				redStart(); break;
				case GET_POS:			sendPos(); break;
				case GET_COMMANDE:		giveCommande(); break;
				case TEST_BRANCHEMENT:	test_branchement(); break;
				/* Déplacements */
				case ALPHA:
				case XY_RAPIDE:
				case XY_COURBE:
				case CALLE_G:
				case CALLE_H:
				case CALLE_D:
					commande[indexLibreCommande] = inByte;
					indexLibreCommande ++; indexLibreCommande %= NB_MAX_OCT_COMMANDES;
					if (!nbArgRestant) nbCommande ++;
			}
			commandePrec = inByte;
		} else {
			nbArgRestant --;
			if (commandePrec >= ALPHA && commandePrec <= CALLE_D) {
				commande[indexLibreCommande] = inByte;
				indexLibreCommande ++; indexLibreCommande %= NB_MAX_OCT_COMMANDES;
			}
			if (!nbArgRestant) {
				switch (commandePrec) {
					case SUPPR:
						suppr(inByte);
						if (!nbCommande) {
							isPaused = 0;
							commandeCourante = FREINAGE;
						}
						break;
					case SET_COEF_FREIN:
						coef_frein =	((unsigned short) argPrec[0] << 8) | 
										inByte;
						break;
					case SET_COEF_FREIN_ALPHA:
						coef_frein_alpha =	((unsigned short) argPrec[0] << 8) | 
											inByte;
						break;
					case SET_DEBUG_SETTING:
						debug_setting =	((unsigned long) argPrec[2] << 24) | 
										((unsigned long) argPrec[1] << 16) | 
										((unsigned short) argPrec[0] << 8) | 
										inByte;
						break;
					case SET_ACCEL_MAX:
						accel_max = inByte;
						break;
					case ALPHA:
					case XY_RAPIDE:
					case XY_COURBE:
					case CALLE_G:
					case CALLE_H:
					case CALLE_D:
						nbCommande ++;
				}
			} else {
				argPrec[nbArgRestant - 1] = inByte;
			}
		}
	}	
}

inline void suppr(unsigned char nbSuppr) {
	if (nbSuppr > nbCommande) nbSuppr = nbCommande;
	short i, indexSuppr = indexCommande;
	// Recherche de l'index inferieur de suppression
	for (i=0; i<nbCommande - nbSuppr; i++)
		switch (commande[indexSuppr]) {
		case XY_RAPIDE:
		case XY_COURBE:
			indexSuppr += 9; indexSuppr %= NB_MAX_OCT_COMMANDES;
			break;
		case ALPHA:
			indexSuppr += 5; indexSuppr %= NB_MAX_OCT_COMMANDES;
		}
	// Suppression
	indexLibreCommande = indexSuppr;
	nbCommande -= nbSuppr;
}

inline void test_branchement(void) {
	//Test de commande min et de sens de rotation : tourne vers la gauche, revient
	long posL1, posR1, posL2, posR2;
	short diffL, diffR;
	
	getQEI(&posL1, &posR1);
	motors_set_speed(0, 1000);
	allumer_del();
	pause_s(3);
	getQEI(&posL2, &posR2);
	diffL = posL2 - posL1;
	diffR = posR2 - posR1;
	if (diffL > (METRE/50) || diffL < -(METRE/50) || diffR < (METRE/50))
		UART_putc(TEST_BRANCHEMENT);
	motors_set_speed(0, -1000);
	eteindre_del();
	pause_s(3);
	
	getQEI(&posL1, &posR1);
	motors_set_speed(1000, 0);
	allumer_del();
	pause_s(3);
	getQEI(&posL2, &posR2);
	diffL = posL2 - posL1;
	diffR = posR2 - posR1;
	if (diffR > (METRE/50) || diffR < -(METRE/50) || diffL < (METRE/50)) {
		UART_putc(TEST_BRANCHEMENT);
		UART_putc(TEST_BRANCHEMENT);
	}	
	motors_set_speed(-1000, 0);
	eteindre_del();
	pause_s(3);
	
	motors_stop();
	SEND_DONE;
}

inline void giveCommande(void) {
	UART_putc(GET_COMMANDE);
	
	UART_putc(commandeCourante);
	if (commandeCourante == XY_RAPIDE || commandeCourante == XY_COURBE) {
		UART_send_tab((unsigned char *) &consigneX, 2);
		UART_send_tab((unsigned char *) &consigneY, 2);
		UART_send_tab((unsigned char *) &consPrecision, 4);
	} else if (commandeCourante == ALPHA) {
		UART_send_tab((unsigned char *) &consAlpha, 2);
		UART_send_tab((unsigned char *) &consPrecision, 2);
	}
}

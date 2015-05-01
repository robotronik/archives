/*-----------------------------------------------------*
 * Protocole de communication avec l'uart               *
 *-----------------------------------------------------*/

#ifndef _MESSAGE_H
#define _MESSAGE_H

#define SIMU

#ifdef SIMU
extern void sendMessageAsserToPsoc(unsigned char c, float dt);
#endif

#define NB_MAX_COMMANDES 128
extern unsigned char commande[NB_MAX_COMMANDES];
extern int nbCommande;
extern int indexCommande;

void message_processing(unsigned char inMessage);	// Appelé à l'interruption de l'UART

/** Interne **/

#define CALLE_G2			0x80
#define CALLE_H2			0x81
#define CALLE_D2			0x82
#define FREINAGE			0x83

/** Commandes **/

// 							0 <= ALPHA < 0x40
#define XY_RAPIDE			0x40
#define XY_COURBE			0x41
#define SUPPR				0x80	// Supprime les N dernières commandes
#define FIN_MATCH				0x81
#define RED 				0x83
#define PURPLE				0x84
#define TEST				0x85	// Connexion bonne
#define GET_POS				0x86
#define CALLE_G				0x87
#define CALLE_H				0x88
#define CALLE_D				0x89

/** Reponses **/

#define DONE				0x00	// Déplacement terminé
//#define TEST				0x85	// Connexion bonne
#define POS					0x86

#endif

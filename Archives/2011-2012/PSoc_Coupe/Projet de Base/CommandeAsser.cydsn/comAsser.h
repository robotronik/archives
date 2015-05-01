#ifndef COMASSER_H_INCLUDED
#define COMASSER_H_INCLUDED

#include "config.h"
#include "cst.h"
#include "signaux.h"
#include "remplirObj.h"

// Messages
#ifndef _MESSAGE_H
#define XY_RAPIDE			0x40
#define XY_COURBE			0x41
#define SUPPR				0x80	// Supprime les N dernières commandes
#define S_90				0x81
#define TEST				0x85	// Connexion bonne
#define GET_POS				0x86
#define CALLE_G				0x87
#define CALLE_H				0x88
#define CALLE_D				0x89

#define DONE				0x00	// Déplacement terminé
//#define TEST				0x85	// Connexion bonne
#define POS					0x86


// Cst
#define TOUR	(int)6773
#endif

#define COULEUR_ROBOT PURPLE


#define MAX_CMD_IN_ASSER 3

#ifdef SIMU
    #include "../Simu/ModuleAsser.h"
#define UART_Asser_PutChar(m) sendMessageAsser(m)
extern void sendMessageAsser(unsigned char c);
#include "../Simu/Data.h"
#else
#define sendMessageAsser(m) UART_Asser_PutChar(m)
#endif


//#define SEND_ASSER_TO_PC

typedef union {
	unsigned char tab[6];
	struct {
		unsigned short x;
		unsigned short y;
		unsigned short angle;
	};
} T_Pos_TabChar;

void initAsser();
void testAsser();
int getNbCmdInAsser();
void sendXY_RAPIDE(unsigned int x, unsigned int y, unsigned int precision);
void sendXY_COURBE(unsigned int x, unsigned int y, unsigned int precision);
void sendALPHA(unsigned int a, unsigned char precision);
void sendSUPPR(unsigned char nbCommande);	// Supprime les N dernières commandes

void sendS_90(); // Seconde 90 -- Xavier a bu trop de gignolet
void sendGET_POS();

void traiterMessageAsser(unsigned char m);
void receivePosAsser(unsigned char m, unsigned char index);
void stopAsserByBrake(); // Arrete les moteurs par freinage, preconise par Xavier

void updatePos();
T_Pos_TabChar getLastPos();

unsigned char isEmptyFileMacro();

void sendDeplGo();
void sendDeplReturn();
#endif // COMASSER_H_INCLUDED

#ifndef COMASSER_H_INCLUDED
#define COMASSER_H_INCLUDED

#include "config.h"
#include "cst.h"
#include "signaux.h"

// Messages
#ifndef _MESSAGE_H


#define PWMG 0
#define PWMD 0b1
#define SGP  0b10
#define SGM  0b11
#define SDP  0b100
#define SDM  0b101


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

extern unsigned char couleurRobotI;

//#define SEND_ASSER_TO_PC

typedef union {
	unsigned char tab[6];
	struct {
		unsigned short x;
		unsigned short y;
		unsigned short angle;
	};
} T_Pos_TabChar;

void sendAsser(unsigned char moteur, char vitesse);
void commande(char m);

#endif // COMASSER_H_INCLUDED

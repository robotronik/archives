/*-----------------------------------------------------*
 * Main :                                              *
 * - inclusion des headers et librairies               *
 * - declaration structure pid                         *
 * - definission des constantes                        *
 *-----------------------------------------------------*/


#ifndef _MAIN_H
#define _MAIN_H

#include <p33FJ128MC802.h>
#include "comAsser.h"
#include "init.h"
#include "servo.h"

#define INT_UART_TX
#define LARGEUR	324								// [mm] distance entre roues codeuses


// parametre pour designer le moteur gauche/droit
typedef enum {FALSE, TRUE} boolean;

typedef struct
{
int type; //0=alpha, 1=delta
double valeur;
}Action;


int abs(int n);
void verifierPosition();
void verifierSharp();
void faire(Action mouvement);
void machine();
int dansTable(int,int);
// Structure associant 2 shorts dans un long pour les QEI
typedef union {
	long all;
	struct {
		short low;
		short high;
	} part;
} T_dividedLong;

typedef union {
	unsigned long all;
	struct {
		short low;
		unsigned short high;
	} part;
} T_dividedULong;

 #include "debug.h"
 #include "init.h"
 #include "uart.h" // couche basse, autonome pour ne pas surcharger le reste

struct Pos {
	int x;
	int y;
	int alpha;
};
typedef struct Pos Pos;

void aller(Pos position);



 #endif

/*-----------------------------------------------------*
 * Main :                                              *
 * - inclusion des headers et librairies               *
 * - declaration structure pid                         *
 * - definission des constantes                        *
 *-----------------------------------------------------*/





#ifndef _MAIN_H
#define _MAIN_H

#include <p33FJ128MC802.h>

// parametre pour designer le moteur gauche/droit
typedef enum {LEFT, RIGHT} Side;
enum {FALSE, TRUE};

// Structure associant 2 shorts dans un long
typedef union {
	struct {
		short low;
		short high;
	} part;
	long all;
} T_dividedLong;

 #include "asser.h"
 #include "debug.h"
 #include "conversion.h"
 #include "init.h"
 #include "message.h"
 #include "motors.h"
 #include "uart.h" // couche basse, autonome pour ne pas surcharger le reste



 #endif

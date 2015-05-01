#ifndef MAIN_H
#define MAIN_H
#define PURPLE_START_X (3000 - 300)
#define PURPLE_START_Y 250
#define PURPLE_START_ALPHA 0
#define RED_START_X 300
#define RED_START_Y 250
#define RED_START_ALPHA (TOUR>>1)

// parametre pour designer le moteur gauche/droit
typedef enum {LEFT, RIGHT} Side;
typedef enum {FALSE, TRUE} boolean;

// Structure associant 2 shorts dans un long
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

#include "asser.h"
#include "debug.h"
#include "conversion.h"
#include "init.h"
#include "uart.h" // couche basse, autonome pour ne pas surcharger le reste

#endif

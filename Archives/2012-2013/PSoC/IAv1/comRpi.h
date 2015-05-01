#ifndef COM_RPI_H
#define COM_RPI_H

#include "comAsser.h"
#include "comXbee.h"

#define CODE_XY 0b10
#define CODE_TIRETTE 0b01
#define CODE_GATEAU 0b01 //peut être égale à code tirette car que dans un sens
#define CODE_COULEUR_PI 0b11
struct TrameDefault
{
    unsigned char 
    val : 6,
	codet : 2;
};
typedef struct TrameDefault TrameDefault;


struct TrameMire
{
    unsigned char 
    M6 : 1,
	M5 : 1,
	M4 : 1,
	M3 : 1,
	M2 : 1,
	M1 : 1,
	codet : 2;
	};
typedef struct TrameMire TrameMire;



struct TrPiPos
{
    unsigned long
    y:12,
	x:12,
	stub:6,
	codet :2;
};
typedef struct TrPiPos TrPiPos;

union TramePi32
{
    Tr4Char tr4char;
    TrPiPos tpos;
    unsigned long l;
};
typedef union TramePi32 TramePi32;

struct TrPiCouleur
{
    unsigned char
    couleur : 1,
    stub : 5,
    codet : 2;
};
typedef struct TrPiCouleur TrPiCouleur;

union TramePi 
{
    
    TrameDefault d;
    TrameMire mire;
	TrPiCouleur couleur;
    unsigned char c;
};
typedef union TramePi TramePi;

void sendRpiTirette();
void sendRpiCouleur();
void sendRpiXY(int x, int y);
void sendRpi32( unsigned long message);
void sendRpi( unsigned short message);
void UART_Rpi_RX_Handler(void);

#endif //COM_RPI_H

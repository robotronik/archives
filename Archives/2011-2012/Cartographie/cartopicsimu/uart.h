#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include "malloc.h"
#include "astar.h"
#include "carto.h"

#ifndef SIMU
#include <p33FJ128MC802.h>
#endif


#ifdef SIMU
extern void* moduleCarto;
extern void receiveMessage(void* destination, unsigned char message);
extern void sendMessage(void* source, unsigned char message, float dt);
#endif

/*La "structure" Trame est une union de 4 champs de bits regroupant
tout les cas de trame possible (emission, reception ...)
La foction sendCoor() envoie les coordonn�es du chemin � suivre, elle utilise
send_message() qui utilise lui m�me UART_putc()*/

struct trData
{
	unsigned long 
		y:11, //poids faible
		x:12,
		numero:6,
		code:3; //poids fort
};
typedef struct trData trData;

struct trDeb
{
	unsigned long 
		longueur:16, //poids faible
		numero:6,
		nb:6,
		code:4; //poids fort
};
typedef struct trDeb trDeb;

struct trChar
{
	unsigned long 
		oooX:8, //poids faible
		ooXo:8,
		oXoo:8,
		Xooo:8; //poids fort
};
typedef struct trChar trChar;

struct trRecep
{
	unsigned long 
		y:11, //poids faible
		x:12,
		numero:6,
		l:1,
		d:1,
		r:1; //poids fort
};
typedef struct trRecep trRecep;

union Trame
{
	trChar charx4;
	trRecep recep;
	trData data;
	trDeb  first;
	unsigned long l;
};
typedef union Trame Trame;

void UART_init();
void UART_putc_carto(unsigned char c);
void send_message_carto(long int message);
void sendCoor(Coor list, int longueur, int longPlusChemin);

#endif // UART_H_INCLUDED




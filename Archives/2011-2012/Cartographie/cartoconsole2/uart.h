#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <stdio.h>

#include "malloc.h"
#include "astar.h"
#include "carto.h"

//extern aTraiter toDo;
//extern aTraiter toDo;

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

union trame
{
	trChar charx4;
	trRecep recep;
	trData data;
	trDeb  first;
	unsigned long l;
};
typedef union trame trame;

void UART_init();
void UART_putc(unsigned char c);
void send_message(long int message);
void sendCoor(Coor list, int longueur, int longPlusChemin);

#endif // UART_H_INCLUDED




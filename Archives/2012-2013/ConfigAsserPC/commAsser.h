/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef COM_ASSER_H
#define COM_ASSER_H

#include "uart.h"

#define CODE_CONFIG_DELTA 0
#define DELTA CODE_CONFIG_DELTA
#define CODE_CONFIG_ALPHA 1
#define ALPHA CODE_CONFIG_ALPHA

#define CODE_CONFIG 0b111
#define CODE_P 0b00
#define P CODE_P
#define CODE_I 0b01
#define I CODE_I
#define CODE_D 0b10
#define D CODE_D


struct AD_K
{
	double delta;
	double alpha;
};
typedef struct AD_K AD_k;


struct Tr4Char
{
	unsigned long
		oooX:8, //poids faible
		ooXo:8,
		oXoo:8,
		Xooo:8; //poids fort
};
typedef struct Tr4Char Tr4Char;

struct TrConf
{
	unsigned long
		dec:8, //poids faible
		ent:16,
		stub:1,
		ad2:1,
		ad1:1,
		pid:2,
		config:3; //poids fort
};
typedef struct TrConf TrConf;

union TrameConfig
{
	Tr4Char tr4Char;
	TrConf conf;
	unsigned long l;
};
typedef union TrameConfig TrameConfig;


void sendConfig(const XBee & xbee, int pid , int ad1 , int ad2, double val);

#endif

//[] END OF FILE

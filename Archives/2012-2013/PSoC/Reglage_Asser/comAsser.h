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

#define CODE_DELTA 0b010
#define CODE_ALPHA 0b101
#define CODE_STOP  0b000

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

struct TrDelta
{
	unsigned short 
		delta:12, //poids faible
		signe:1, // 1 négatif, 0 positif
		codet:3; //poids fort
};
typedef struct TrDelta TrDelta;

struct TrAlpha
{
	unsigned short 
		alpha:9, //poids faible
		signe:1,
		stub:3,
		codet:3; //poids fort
};
typedef struct TrAlpha TrAlpha;

struct TrShort
{
	unsigned short 
		oX:8, //poids faible
		Xo:8; //poids fort
};
typedef struct TrShort TrShort;

union TrameAsser
{
	TrDelta trDelta;
	TrAlpha trAlpha;
	TrShort trShort;
	unsigned short us;
};
typedef union TrameAsser TrameAsser;


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

void sendAsser2( unsigned short message);
void sendAsser4( unsigned long message);
void sendDelta( short delta );
void sendAlpha( short alpha );
void sendStop(void);
void sendConfig( int pid , int ad1 , int ad2, double val);
void UART_Asser_RX_Handler(void);

#endif

//[] END OF FILE

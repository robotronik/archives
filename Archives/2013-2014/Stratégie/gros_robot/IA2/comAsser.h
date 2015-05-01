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

#define CODE_DELTA 0b010 //0x4 ou 0x5
#define CODE_ALPHA 0b1010 //0xA
#define CODE_ALPHA_ABS 0b1011 //0xB
#define CODE_STOP  0b0000 //0x0
#define CODE_GET_XY 0b1100 //0xC
#define CODE_GET_ALPHA 0b1101 //0xD
#define CODE_SET_XY 0b0011 //0x3
#define CODE_SET_ALPHA 0b0010 //0x2
#define CODE_PRES_GROS 0b0110 //0x6
#define CODE_PRES_PRECIS 0b0111 //0x7

#define CODE_DERAPAGE 0b1000 //0x8

#define CODE_FREE_WHEEL 0b1001 //0x9

#define DONE_ASSER 0b0001

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
		stub:2,
		codet:4; //poids fort
};
typedef struct TrAlpha TrAlpha;

struct TrGenShort
{
	unsigned short 
		stub:12,
		codet:4; //poids fort
};
typedef struct TrGenShort TrGenShort;

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
	TrGenShort trGenShort;
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

struct TrPos
{
	unsigned long 
		y:12,
		x:12,
		stub:4,
		codet:4;
};
typedef struct TrPos TrPos;

union Trame32
{
	Tr4Char tr4Char;
	TrPos tpos;
	unsigned long l;
};
typedef union Trame32 Trame32;





void sendAsser( unsigned short message);
void sendDelta( short delta );
void sendAlpha( short alpha );
void sendAlphaAbs( short alpha );
void sendStop(void);
void UART_Asser_RX_Handler(void);
void waitDoneAsser(void);//obsolete should not be used
void getAlpha(void);
void getXY(void);
void sendSetXY(int x, int y);
void sendSetAlpha(short alpha);
void sendPresGros(void);
void sendPresPrecis(void);
void sendFreeWheel(void);
void resetDoneAsser(void);
void resetDerapage(void);
void waitRequestGetAlphaSafe(void);
void waitRequestGetXYSafe(void);

#endif


//[] END OF FILE

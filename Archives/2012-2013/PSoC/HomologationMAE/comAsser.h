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
#define CODE_STOP  0b0001
#define CODE_GET_XY 0b100
#define CODE_GET_ALPHA 0b001

#define DONE_ASSER 0x0D
#define ALPHA_ASSER 0xAA

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

struct TrPos
{
	unsigned long 
		y:12,
		x:12,
		stub:5,
		codet:3;
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
void sendStop(void);
void UART_Asser_RX_Handler(void);
void waitDoneAsser(void);
void getAlpha(void);
void getXY(void);

#endif

//[] END OF FILE

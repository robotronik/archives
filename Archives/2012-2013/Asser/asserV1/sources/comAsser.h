/*-----------------------------------------------------*
 * Protocole de communication avec l'uart               *
 *-----------------------------------------------------*/
 
#ifndef _MESSAGE_H
#define _MESSAGE_H

#define DEBUG_PRINTF(message, ...)

/* Les diff�rents code ci dessous permettent de diff�rencier les commandes
 envoy�/recu au pSoC.
 Au depart ces codes �tait sur 3 bits, ils sont maintenent sur 4 pour avoir
 plus de commandes.
 Sauf CODE_DELTA qui est restait sur 3, car il n'y a pas la place dans la trame
 donc le code de delta est en fait soit 0b0100 soit 0b0101 (0x4 ou 0x5)*/
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

#define DONE_ASSER 0b0001

struct TrDelta
{
	unsigned short
		delta:12, //poids faible
		signe:1, // 1 n�gatif, 0 positif
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


void sendDelta( short delta );
void sendAlpha( short alpha );
void sendAlphaAbs( short alpha );
void sendStop(void);
void waitDoneAsser(void);
void getAlpha(void);
void getXY(void);
void sendSetXY(int x, int y);
void sendSetAlpha(short alpha);
void sendPresGros(void);
void sendPresPrecis(void);
void message_processing(unsigned char inMessage);	// Appel�e � l'interruption de l'UART
void waitDoneAsser(void);

#endif

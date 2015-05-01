#ifndef UART_H
#define UART_H

/************************/
//Protocole de communication 
#define CODE_TIRETTE 0b01
#define CODE_POS 0b10
#define CODE_COULEUR 0b11
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
struct TrCouleur
{
    unsigned char
    couleur : 1,
    stub : 5,
    codet : 2;
};
typedef struct TrCouleur TrCouleur;
struct Tr4Char
{
    unsigned long
    oooX:8,
	ooXo:8,
	oXoo:8,
	Xooo:8;
};
typedef struct Tr4Char Tr4char; 

struct TrPos
{
    unsigned long
    y:12,
	x:12,
	stub:6,
	codet :2;
};
typedef struct TrPos TrPos;

union Trame4
{
    Tr4char tr4char;
    TrPos tpos;
    unsigned long l;
};

typedef union Trame4 Trame4;

union Trame 
{
    
    TrameDefault d;
    TrameMire mire;
	TrCouleur couleur;
    unsigned char c;
};
typedef union Trame Trame;

#endif 

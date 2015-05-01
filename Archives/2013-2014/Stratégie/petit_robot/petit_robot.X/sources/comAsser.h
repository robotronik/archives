/*-----------------------------------------------------*
 * Protocole de communication avec l'uart               *
 *-----------------------------------------------------*/

#ifndef COMASSER_H
#define COMASSER_H

#define DEBUG_PRINTF(message, ...)

/* Les différents code ci dessous permettent de différencier les commandes
 envoyé/recu au pSoC.
 Au depart ces codes était sur 3 bits, ils sont maintenent sur 4 pour avoir
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

typedef struct {
    unsigned short
    delta:12, //poids faible
    signe:1, // 1 négatif, 0 positif
    codet:3; //poids fort
} TrDelta;

typedef struct {
    unsigned short
        alpha:9, //poids faible
        signe:1,
        stub:2,
        codet:4; //poids fort
} TrAlpha;

typedef struct {
    unsigned short
        stub:12,
        codet:4; //poids fort
} TrGenShort;

typedef struct {
    unsigned short
        oX:8, //poids faible
        Xo:8; //poids fort
} TrShort;

typedef union {
    TrDelta trDelta;
    TrAlpha trAlpha;
    TrShort trShort;
    TrGenShort trGenShort;
    unsigned short us;
} TrameAsser;

typedef struct {
    unsigned long
        oooX:8, //poids faible
        ooXo:8,
        oXoo:8,
        Xooo:8; //poids fort
} Tr4Char;

typedef struct {
    unsigned long
        y:12,
        x:12,
        stub:4,
        codet:4;
} TrPos;

typedef union {
    Tr4Char tr4Char;
    TrPos tpos;
    unsigned long l;
} Trame32;


void UART_init_asser();

// Mouvement
void sendDelta(short delta);
void sendAlpha(short alpha);
void sendAlphaAbs(short alpha);

// Stop et wait…
void sendStop(void);
void waitDoneAsser(void);

// Obtention des positions
void getAlpha(void);
void getXY(void);

// Réglage des positions
void sendSetXY(int x, int y);
void sendSetAlpha(short alpha);

// Non utilisés
void sendPresGros(void);
void sendPresPrecis(void);

/** récupère les messages provenant de l’assert
 *
 * \param inMessage message reçu
 * \warnig Cette fonction est **uniquement** appelé par une interruption
 */
void message_processing(unsigned char inMessage);    // Appelée à l'interruption de l'UART

void waitDoneAsser(void);

/* Donne la position en absolue de l’angle du robot en degré
 * \todo Quelle est la référence ? sur la droite, comme en trigo ?
 * On a supposé dans le reste du code sens horaire, 0 = axe R-J, à changer donc en conséquence
 */
Angle getAlphaAbs();

/* Renvoie la position sur l’axe x du robot
 */
Coordonne_mm getPosX();

/* Renvoie la position sur l’axe y du robot
 */
Coordonne_mm getPosY();

/* Renvoie vraie si l’assert à terminée son job ! (le robot à atteind sa destination
 */
bool assertAtteint();

/* Attend que le match démarre
 */
void wait_match_start();
#endif /* COMASSER_H */

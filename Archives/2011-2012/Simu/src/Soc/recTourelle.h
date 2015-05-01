#ifndef RECTOURELLE_H_INCLUDED
#define RECTOURELLE_H_INCLUDED

#define TAILLE_FILE 16 // 16 est un maximum, sinon il faut changer le type de la structure FILE


#include "cst.h"
#include "config.h"
#include "signaux.h"

#ifdef SIMU
#include "../Simu/ModuleTourelle.h"
#else
#define sendMessageTourelle(c) UART_XBee_PutChar(c)
#endif

typedef struct
{
    unsigned long long  int // 64 bits
        X : 12,
        Y : 11,
        t : 41; // Tellement laaarge
} EntreeFilePos;

typedef struct
{
    EntreeFilePos file[TAILLE_FILE];
    unsigned long long int lastT;
    unsigned char
        debut : 4,
        taille : 4;
} FilePos;

void initPosRobot();
void pushPosRobot(EntreeFilePos entree, unsigned char index);
EntreeFilePos popPosRobot(unsigned char index);
char getSizePosRobot(unsigned char index);

// traitant IT
void startRefreshTourelle();
void traiterMessageTourelle(unsigned char message);

#endif // RECTOURELLE_H_INCLUDED

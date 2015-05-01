/* 
 * File:   comXbee.h
 * Author: Dan
 *
 * Created on 16 avril 2013, 19:02
 */

#ifndef COMXBEE_H
#define COMXBEE_H

#include "comAsser.h"

#define CODE_TIRETTE 0b0000
#define CODE_XY 0b0001
#define CODE_COULEUR 0b0010
#define CODE_FIN 0b0011
#define CODE_XBEE_GET_XY 0b0100


struct TrCouleur
{
    unsigned char
    couleur : 1,
    stub : 3,
    codet : 4;
};
typedef struct TrCouleur TrCouleur;

struct TrDefault
{
    unsigned char
    stub : 4,
    codet : 4;
};
typedef struct TrDefault TrDefault;

union TrXbee
{
    TrCouleur couleur;
    TrDefault d;
    char c;
};
typedef union TrXbee TrXbee;

void sendXY();
void message_processing_xbee(unsigned char inMessage);
#endif /* COMXBEE_H */


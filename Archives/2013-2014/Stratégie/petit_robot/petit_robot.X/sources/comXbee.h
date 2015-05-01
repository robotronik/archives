/* 
 * File:   comXbee.h
 * Author: Dan
 *
 * Created on 16 avril 2013, 19:02
 */

#ifndef COMXBEE_H
#define COMXBEE_H

#define CODE_TIRETTE 0b0000
#define CODE_XY 0b0001
#define CODE_COULEUR 0b0010
#define CODE_FIN 0b0011
#define CODE_XBEE_GET_XY 0b0100


typedef struct {
    unsigned char
        couleur : 1,
        stub : 3,
        codet : 4;
} TrCouleur;

typedef struct {
    unsigned char
        stub : 4,
        codet : 4;
} TrDefault;

typedef union {
    TrCouleur couleur;
    TrDefault d;
    char c;
} TrXbee;

/* Envoie la position par XBee
 */
void UART_init_xbee();
void sendXY();

/** récupère les messages provenant du xbee
 *
 * \param inMessage message reçu
 * \warnig Cette fonction est **uniquement** appelé par une interruption
 */
void message_processing_xbee(unsigned char inMessage);
#endif /* COMXBEE_H */

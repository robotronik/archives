/* 
 * File:   comXbee.c
 * Author: Dan
 *
 * Created on 16 avril 2013, 19:01
 */

#include "main.h"
#include "comAsser.h"
#include "comXbee.h"
#include "uart_xbee.h"

#include "debug.h"

extern volatile char tirette;
extern volatile Pos robot1;
extern volatile Pos robotNous;
extern Couleur couleur;

void UART_init_xbee() {
    TRIS_RX_XBEE = 1;
    TRIS_TX_XBEE = 0;

    // Aterminer
    RPOR4bits.RP8R = 0b00101;                // Tx2 -> RP8
    RPINR19bits.U2RXR = 7;                    // RP7 -> Rx2


    // Low speed : BRG = 79,23 MHz / 32 / Baudrate - 1
    U2MODEbits.BRGH = 1;    // High speed : BRG = 79,23 MHz / 8 / Baudrate - 1
    U2BRG = 85;             // BAUD Rate Setting for 115200 gives 115160 bauds

    #ifdef INT_UART_TX
        U2STAbits.UTXISEL1 = 1;    // Interrupt on empty FIFO, last byte is being sent
        U2STAbits.UTXISEL0 = 0;    //                      "
    #endif //#ifdef INT_UART_TX

        IFS1bits.U2RXIF = 0;     // On evite des interruptions � l'activation
        IEC1bits.U2RXIE = 1;    // Activation de l'interruption sur r�ceptions
    #ifdef INT_UART_TX
        IFS1bits.U2TXIF = 0;
        IEC1bits.U2TXIE = 1;    // Activation de l'interruption sur l'envoie
    #endif //#ifdef INT_UART_TX
    //IFS4bits.U1EIF = 0;
    //IEC4bits.U1EIE = 1;    // Activation de l'interruption sur erreurs

    U2MODEbits.UARTEN = 1;     // Enable UART
    U2STAbits.UTXEN = 1;     // Enable UART TX
}

void sendXY() {
    Trame32 t;
    t.tpos.codet = CODE_XY;
    t.tpos.x = robotNous.x;
    t.tpos.y = robotNous.y;
    send_xbee_long(t.l);
}

void message_processing_xbee(unsigned char inMessage) {
    static int cpt = 0;
    static unsigned char messageType;
    TrXbee t;
    static Trame32 t32;


    // affiche_debug(DEBUG_MSG | 0b01110000);

    if (cpt == 0) {

        // affiche_debug(DEBUG_MSG | 0b01110011);

        t.c = inMessage;
        t32.tr4Char.Xooo = inMessage;
        switch (t.d.codet) {
            case CODE_COULEUR:  couleur = t.couleur.couleur;
                break;

            case CODE_TIRETTE:  tirette = 1;
                break;

            case CODE_XBEE_GET_XY:  sendXY();
                break;

            case CODE_XY :      messageType = CODE_XY;
                cpt++;
                break;
        }
    }
    else if (cpt == 1) {

        // affiche_debug(DEBUG_MSG | 0b01111100);

        t32.tr4Char.oXoo = inMessage;
        switch (messageType) {
            case CODE_XY: cpt++;
                break;
        }
    }
    else if (cpt == 2) {

        // affiche_debug(DEBUG_MSG | 0b01111111);

        cpt++;
        t32.tr4Char.ooXo = inMessage;
    }
    else {

        // affiche_debug(DEBUG_MSG | 0b01110011);

        cpt = 0;
        t32.tr4Char.oooX = inMessage;
        switch (messageType) {
            case CODE_XY: robot1.x = t32.tpos.x;
                robot1.y = t32.tpos.y;
                break;
        }
    }

    // affiche_debug(DEBUG_MSG | 0b01110011);

}






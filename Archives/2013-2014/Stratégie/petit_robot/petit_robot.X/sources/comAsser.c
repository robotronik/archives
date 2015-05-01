/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/

#include <stdlib.h>

#include "main.h"
#include "comAsser.h"
#include "uart.h"
#include "debug.h"

extern volatile int doneAsser;
extern volatile int requestGetAlpha;
extern volatile int requestGetXY;
extern volatile Pos robotNous;

void UART_init_asser() {
    doneAsser = 1;
    TRIS_TX_ASSER = 0;
    TRIS_RX_ASSER = 1;

    AD1PCFGLbits.PCFG5 = 1;    // Désactivation de l'entrée analogique !!!
    RPOR1bits.RP2R = 3;                            // Tx1 -> RP2
    RPINR18bits.U1RXR = 3;                        // RP3 -> Rx1

    //U1MODEbits.LPBACK = 1;    // Debug : Tx1 -> Rx1

    // Low speed : BRG = 79,23 MHz / 32 / Baudrate - 1
    U1MODEbits.BRGH = 1;    // High speed : BRG = 79,23 MHz / 8 / Baudrate - 1
    U1BRG = 85;             // BAUD Rate Setting for 115200 gives 115160 bauds

    #ifdef INT_UART_TX
        U1STAbits.UTXISEL1 = 1;    // Interrupt on empty FIFO, last byte is being sent
        U1STAbits.UTXISEL0 = 0;    //                      "
    #endif //#ifdef INT_UART_TX

    IFS0bits.U1RXIF = 0;     // On evite des interruptions � l'activation
    IEC0bits.U1RXIE = 1;     // Activation de l'interruption sur r�ceptions
    #ifdef INT_UART_TX
        IFS0bits.U1TXIF = 0;
        IEC0bits.U1TXIE = 1;    // Activation de l'interruption sur l'envoie
    #endif //#ifdef INT_UART_TX
    //IFS4bits.U1EIF = 0;
    //IEC4bits.U1EIE = 1;    // Activation de l'interruption sur erreurs

    U1MODEbits.UARTEN = 1;     // Enable UART
    U1STAbits.UTXEN = 1;     // Enable UART TX
}

void sendDelta(short delta) {
    doneAsser=0;
    TrameAsser t;
    t.trDelta.delta = abs(delta);
    t.trDelta.signe = (delta < 0);
    t.trDelta.codet = CODE_DELTA;

    sendAsser(t.us);
    DEBUG_PRINTF("sendDelta %d", delta);
}

void sendAlpha(short alpha) {
    doneAsser=0;
    TrameAsser t;
    t.trAlpha.alpha = abs(alpha);
    t.trAlpha.signe = (alpha < 0);
    t.trAlpha.codet = CODE_ALPHA;

    sendAsser(t.us);
    DEBUG_PRINTF("sendAlpha %d", alpha);
}

void sendAlphaAbs(short alpha) {
    doneAsser=0;
    TrameAsser t;
    t.trAlpha.alpha = abs(alpha);
    t.trAlpha.signe = (alpha < 0);
    t.trAlpha.codet = CODE_ALPHA_ABS;

    sendAsser(t.us);
    DEBUG_PRINTF("sendAlphaAbs %d", alpha);
}


void getXY(void) {
    TrameAsser t;
    t.trGenShort.codet = CODE_GET_XY;
    requestGetXY = 1;
    DEBUG_PRINTF("getXYRequest");
    sendAsser(t.us);
}

void getAlpha(void) {
    TrameAsser t;
    t.trGenShort.codet = CODE_GET_ALPHA;
    requestGetAlpha = 1;
    DEBUG_PRINTF("getAlphaRequest");
    sendAsser(t.us);
}

void sendSetXY(int x, int y) {
    Trame32 t;
    t.tpos.codet = CODE_SET_XY;
    t.tpos.x = x;
    t.tpos.y = y;
    sendAsser32(t.l);
    DEBUG_PRINTF("sendSetXY:%d,%d", x, y);
}

void sendSetAlpha(short alpha) {
    TrameAsser t;
    t.trAlpha.alpha = abs(alpha);
    t.trAlpha.signe = (alpha < 0);
    t.trAlpha.codet = CODE_SET_ALPHA;
    sendAsser(t.us);
    DEBUG_PRINTF("sendSetAlpha:%d", alpha);
}


// Non utilisés
void sendPresGros(void) {
    TrameAsser t;
    t.trGenShort.codet = CODE_PRES_GROS;
    sendAsser(t.us);
    DEBUG_PRINTF("sendPresGros");
}

void sendPresPrecis(void) {
    TrameAsser t;
    t.trGenShort.codet = CODE_PRES_PRECIS;
    sendAsser(t.us);
    DEBUG_PRINTF("sendPresPrecis");
}


void message_processing(unsigned char inMessage) {
    static int cpt = 0;
    static unsigned char messageType;
    static TrameAsser t16;
    static Trame32 t32;

//    while (1) {
//        affiche_debug(DEBUG_MSG | 0b11110000);
//        affiche_debug(DEBUG_MSG | 0b00001111);
//    }

    if (cpt == 0) {
        t16.trShort.Xo = inMessage;
        t32.tr4Char.Xooo = inMessage;
        switch (t16.trGenShort.codet) {
            case (DONE_ASSER): //UART_XBee_PutString(" Fin Asser Vrai");
                doneAsser = 1;
                messageType = DONE_ASSER;
                DEBUG_PRINTF("DoneAsser");
                break;
            case (CODE_DERAPAGE): DEBUG_PRINTF("Derapage");
                break;
            case CODE_GET_ALPHA: cpt++;
                messageType = CODE_GET_ALPHA;
                break;
            case CODE_GET_XY: cpt++;
                messageType = CODE_GET_XY;
                break;
        }
    }
    else if (cpt == 1) {
        t16.trShort.oX = inMessage;
        t32.tr4Char.oXoo = inMessage;
        switch (messageType) {
            case CODE_GET_ALPHA: cpt = 0;
                if (t16.trAlpha.signe == 1){
                    //posAlpha = -t16.trAlpha.alpha;
                    robotNous.alpha = -t16.trAlpha.alpha;
                }   
                else{
                    //posAlpha = t16.trAlpha.alpha;
                    robotNous.alpha = t16.trAlpha.alpha;
                }
                requestGetAlpha = 0;
                DEBUG_PRINTF("GetAlpha : %d", robotNous.alpha);
                break;
            case CODE_GET_XY:
                cpt++;
                break;
        }
    }
    else if (cpt == 2) {
        cpt++;
        t32.tr4Char.ooXo = inMessage;
    }
    else {
        cpt = 0;
        t32.tr4Char.oooX = inMessage;
        switch (messageType) {
            case CODE_GET_XY:
                //posX = t32.tpos.x;
                //posY = t32.tpos.y;
                robotNous.x = t32.tpos.x;
                robotNous.y = t32.tpos.y;
                requestGetXY = 0;
                DEBUG_PRINTF("GetXY : %d,%d", robotNous.x, robotNous.y);
                //On envoit notre position au Pi
                break;
        }
    }
}

void sendStop(void) {
    doneAsser=0;
    TrameAsser t;
    t.trShort.Xo = CODE_STOP;

    sendAsser(t.us);
    DEBUG_PRINTF("sendStop");
}

void waitDoneAsser(void) {
    while(!doneAsser);
}

Angle getAlphaAbs() {
    getXY();
    while (requestGetAlpha) { ; }
    return robotNous.alpha;
}

Coordonne_mm getPosX() {
    getXY();
    while (requestGetXY) { ; }
    return robotNous.x;
}

Coordonne_mm getPosY() {
    getXY();
    while (requestGetXY) { ; }
    return robotNous.y;
}

bool assertAtteint() {
    return doneAsser;
}

extern volatile char tirette;
// tirette est une variable globale, modifié par la fonction ci-dessous

void wait_match_start() {
    affiche_debug(DEBUG_0); 
    while (!tirette) { ; }
    affiche_debug(DEBUG_1);
}

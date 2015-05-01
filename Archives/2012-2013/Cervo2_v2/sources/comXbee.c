/* 
 * File:   comXbee.c
 * Author: Dan
 *
 * Created on 16 avril 2013, 19:01
 */

#include "comXbee.h"
#include "constantes.h"
#include "uart_xbee.h"
#include "main.h"
/*
 * 
 */
extern volatile char tirette;
extern volatile Pos robot1;
extern volatile Pos robotNous;
extern volatile char fin;
extern Couleur couleur;

void sendXY()
{
    Trame32 t;
    t.tpos.codet = CODE_XY;
    t.tpos.x = robotNous.x;
    t.tpos.y = robotNous.y;
    send_xbee_long(t.l);
}

void message_processing_xbee(unsigned char inMessage)
{
    static int cpt = 0;
    static unsigned char messageType;
    TrXbee t;
    static Trame32 t32;

            
    if (cpt == 0)
    {
        t.c = inMessage;
        t32.tr4Char.Xooo = inMessage;
        switch (t.d.codet)
        {

        case CODE_COULEUR:
            couleur = t.couleur.couleur;

        break;

        case CODE_FIN :
               // fin = 1;
        break;

        case CODE_TIRETTE:
            tirette = 1;

        break;
        case CODE_XBEE_GET_XY:
            sendXY();
         break;

         case CODE_XY :
              messageType = CODE_XY;
              cpt++;
         break;
        }
    }
    else if (cpt == 1)
    {
        t32.tr4Char.oXoo = inMessage;
        switch (messageType)
        {
        case CODE_XY:
            cpt++;
            break;
        }
    }
    else if (cpt == 2)
    {
        cpt++;
        t32.tr4Char.ooXo = inMessage;
    }
    else
    {
        cpt = 0;
        t32.tr4Char.oooX = inMessage;
        switch (messageType)
        {
        case CODE_XY:
            robot1.x = t32.tpos.x;
            robot1.y = t32.tpos.y;
            break;
        }
    }
}






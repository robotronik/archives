/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/

#include "main.h"
#include "comAsser.h"
#include "uart.h"

volatile int doneAsser = 1;

volatile int requestGetAlpha = 0;
volatile int requestGetXY = 0;

extern volatile Pos robotNous;

void sendDelta(short delta)
{
    doneAsser=0;
    TrameAsser t;
    if (delta < 0)
    {
        t.trDelta.delta = -delta;
        t.trDelta.signe = 1;
    }
    else
    {
        t.trDelta.delta = delta;
        t.trDelta.signe = 0;
    }
    t.trDelta.codet = CODE_DELTA;

    sendAsser(t.us);
    DEBUG_PRINTF("sendDelta %d", delta);
}

void sendAlpha(short alpha)
{
    doneAsser=0;
    TrameAsser t;
    if (alpha < 0)
    {
        t.trAlpha.alpha = -alpha;
        t.trAlpha.signe = 1;
    }
    else
    {
        t.trAlpha.alpha = alpha;
        t.trAlpha.signe = 0;
    }
    t.trAlpha.codet = CODE_ALPHA;

    sendAsser(t.us);
    DEBUG_PRINTF("sendAlpha %d", alpha);
}

void sendAlphaAbs(short alpha)
{
    doneAsser=0;
    TrameAsser t;
    if (alpha < 0)
    {
        t.trAlpha.alpha = -alpha;
        t.trAlpha.signe = 1;
    }
    else
    {
        t.trAlpha.alpha = alpha;
        t.trAlpha.signe = 0;
    }
    t.trAlpha.codet = CODE_ALPHA_ABS;

    sendAsser(t.us);
    DEBUG_PRINTF("sendAlphaAbs %d", alpha);
}

void sendStop(void)
{
    doneAsser=0;
    TrameAsser t;
    t.trShort.Xo = CODE_STOP;

    sendAsser(t.us);
    DEBUG_PRINTF("sendStop");
}

void getXY(void)
{
    TrameAsser t;
    t.trGenShort.codet = CODE_GET_XY;
    requestGetXY = 1;
    DEBUG_PRINTF("getXYRequest");
    sendAsser(t.us);


}

void sendSetXY(int x, int y)
{
    Trame32 t;
    t.tpos.codet = CODE_SET_XY;
    t.tpos.x = x;
    t.tpos.y = y;
    sendAsser32(t.l);
    DEBUG_PRINTF("sendSetXY:%d,%d", x, y);
}

void sendSetAlpha(short alpha)
{
    TrameAsser t;
    if (alpha < 0)
    {
        t.trAlpha.alpha = -alpha;
        t.trAlpha.signe = 1;
    }
    else
    {
        t.trAlpha.alpha = alpha;
        t.trAlpha.signe = 0;
    }
    t.trAlpha.codet = CODE_SET_ALPHA;
    sendAsser(t.us);
    DEBUG_PRINTF("sendSetAlpha:%d", alpha);
}

void getAlpha(void)
{
    TrameAsser t;
    t.trGenShort.codet = CODE_GET_ALPHA;
    requestGetAlpha = 1;
    DEBUG_PRINTF("getAlphaRequest");
    sendAsser(t.us);


}

void sendPresGros(void)
{
    TrameAsser t;
    t.trGenShort.codet = CODE_PRES_GROS;
    sendAsser(t.us);
    DEBUG_PRINTF("sendPresGros");
}

void sendPresPrecis(void)
{
    TrameAsser t;
    t.trGenShort.codet = CODE_PRES_PRECIS;
    sendAsser(t.us);
    DEBUG_PRINTF("sendPresPrecis");
}

void message_processing(unsigned char inMessage)
{
    static int cpt = 0;
    static unsigned char messageType;
    static TrameAsser t16;
    static Trame32 t32;

    if (cpt == 0)
    {
        t16.trShort.Xo = inMessage;
        t32.tr4Char.Xooo = inMessage;
        switch (t16.trGenShort.codet)
        {
        case (DONE_ASSER):
            //UART_XBee_PutString(" Fin Asser Vrai");
            doneAsser = 1;
            messageType = DONE_ASSER;
            DEBUG_PRINTF("DoneAsser");
            break;
        case (CODE_DERAPAGE):
            DEBUG_PRINTF("Derapage");
            break;
        case CODE_GET_ALPHA:
            cpt++;
            messageType = CODE_GET_ALPHA;
            break;
        case CODE_GET_XY:
            cpt++;
            messageType = CODE_GET_XY;
            break;
        }
    }
    else if (cpt == 1)
    {
        t16.trShort.oX = inMessage;
        t32.tr4Char.oXoo = inMessage;
        switch (messageType)
        {
        case CODE_GET_ALPHA:
            cpt = 0;
            if (t16.trAlpha.signe == 1)
                //posAlpha = -t16.trAlpha.alpha;
                robotNous.alpha = -t16.trAlpha.alpha;
            else
                //posAlpha = t16.trAlpha.alpha;
                robotNous.alpha = t16.trAlpha.alpha;
            requestGetAlpha = 0;
            DEBUG_PRINTF("GetAlpha : %d", robotNous.alpha);
            break;
        case CODE_GET_XY:
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

void waitDoneAsser(void)
{

	while(!doneAsser);

}

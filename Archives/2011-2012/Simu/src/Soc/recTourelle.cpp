#include "recTourelle.h"

static FilePos file[nombreTourelles];
static unsigned char updatedTourelle = 1;
static unsigned char posAsked = 0;

static unsigned short buffX[nombreTourelles] = {0};
static unsigned short buffY[nombreTourelles] = {0};

const unsigned char adrTourelles[nombreTourelles] =
{
    0b010 // Robot adverse
};
unsigned int precisionTourelle[nombreTourelles] =  // en mm
{
    PRECISION_ACCELEROMETRE    // Accelerometre precis au centimetre
};

void initPosRobot()
{
    int i;
    for(i=0; i<nombreTourelles; i++)
    {
        file[i].debut = 0;
        file[i].taille = 0;
        file[i].lastT = 0;
    }
}

void pushPosRobot(EntreeFilePos entree, unsigned char index)
{
    if(entree.t - file[index].lastT > 100)
    {
        file[index].lastT = entree.t;
        file[index].file[(file[index].debut+file[index].taille)%NB_DEPL_MAX] = entree;
        file[index].taille++;
    }
}

EntreeFilePos popPosRobot(unsigned char index)
{
    unsigned char i = file[index].debut;
    file[index].debut = (file[index].debut + 1)%NB_DEPL_MAX;
    file[index].taille--;
    return file[index].file[i];
}

char getSizePosRobot(unsigned char index)
{
    return file[index].taille;
}


void startRefreshTourelle()
{
    if(updatedTourelle)
    {
        updatedTourelle = 0;
        sendMessageTourelle(0b00111111);
    }
}

static void swapBufferTourelle()    // Va recopier les infos recues dans les files
{
    if(isTest())
        validerTourelle();
    EntreeFilePos e;
    unsigned char i;
    e.t = getTime();
    for(i=0; i<nombreTourelles; i++)
    {
        if(buffX[i] >= 32917 && buffX[i] <= 65535)
            e.X = 0;
        else if(buffX[i] > 300 && buffX[i] < 32917)
            e.X = 3000;
        else
            e.X = buffX[i]*10;

        if(buffY[i] >= 32667 && buffY[i] <= 65535)
            e.Y = 0;
        else if(buffY[i] > 200 && buffY[i] < 32667)
            e.Y = 2000;
        else
            e.Y = buffY[i]*10;

        pushPosRobot(e, i);
    }
}

void traiterMessageTourelle(unsigned char message)
{
    static char tramesReceived = 0;
    if((message & 0b11100000))
        return; // Non destine

    if(!updatedTourelle)
    {
        if(message != 0b00011111)
            return ; // What The Fuck Was This Message ???
        updatedTourelle = 1;
        // On recupere l'adresse du premier
        posAsked = 0;
        tramesReceived = 0;
        unsigned char trame = (adrTourelles[posAsked]<<5) + 0b11111;
        sendMessageTourelle(trame);
    }
    else
    {
        // On traite la trame
        switch(tramesReceived)
        {
            case 0 : // X1
                buffX[posAsked] = message<<5; break;
            case 1 : // X2
                buffX[posAsked] += (message&0b11111); break;
            case 2 : // Y1
                buffY[posAsked] = message<<5; break;
            case 3 : // X2
                buffY[posAsked] += (message&0b11111); break;
            default : break; // non passe
        }
        // On envoie la suivante
        tramesReceived++;
        if(tramesReceived == 4)
        {
            // On peut recuperer la position restante
            posAsked++;
            if(posAsked == nombreTourelles)
            {
                swapBufferTourelle();
                startRefreshTourelle();
            }
            else
            {
                tramesReceived = 0;
                unsigned char trame = (adrTourelles[posAsked]<<5) + 0b11111;
                sendMessageTourelle(trame);
            }
        }
    }
}

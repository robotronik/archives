#include "ModuleTourelle.h"

using namespace Simu;

ModuleTourelle* moduleTourelle = NULL;

ModuleTourelle::ModuleTourelle() : Module()
{
    moduleTourelle = this;
    for(int i=0; i<nombreTourelles; i++)
        precisionOfTourelle[i] = 1;
    precisionOfTourelle[0b010] = 10;
}

void ModuleTourelle::run(float dt)
{
    // Changer la precision
    t += dt;
    precisionOfTourelle[0b010] +=  (rand()%100 < 1) ? 1 : 0;

    traiterMessagesRecus();

    // Ne travaille pas en interne, ne fonctionne que par interruption

    envoyerMessages();
}

void ModuleTourelle::traiterUnMessage(unsigned char message)
{
    // Ici, faire les calculs et envoyer au Psoc
    if(message == 0b00111111)
        sendMessageTourelleToSoc(0b00011111, 0);
    else if((message & 0b00011111) == 0b00011111)
    {
        unsigned char adrVise = message >> 5;
        unsigned char tourelleVise;
        for(tourelleVise=0; tourelleVise<nombreTourelles; tourelleVise++)
        {
            if(adrTourelles[tourelleVise] == adrVise)
                break;
        }
        if(tourelleVise < nombreTourelles) // Adresse Valide, mappee
        {
            // Si c'est l'accelerometre  0b010, on ne change pas le sens
            // Sinon, on inverse si on est rouge
            unsigned short x, y;
            if(Simu::DataSimu::getInstance() == NULL)
            {
                x = 0;
                y = 0;
            }
            else
            {
                float X,Y;
                switch(tourelleVise)
                {
                    case INDEX_TOURELLE_ADV_1 :
                        if(couleurRobotI == RED)
                        {
                            X = Simu::DataSimu::getInstance()->getRobotViolet().getPos().X;
                            Y = Simu::DataSimu::getInstance()->getRobotViolet().getPos().Z;
                        }
                        else
                        {
                            X = Simu::DataSimu::getInstance()->getRobotRouge().getPos().X;
                            Y = Simu::DataSimu::getInstance()->getRobotRouge().getPos().Z;
                        }
                    break;
                    default : return;
                }

                // Passage 3D -> 2D (cm)
                x = (unsigned short)((15 - X)*10);
                y = (unsigned short)((10 - Y)*10);

                if(adrVise != 0b010 && couleurRobotI == RED) // Accel et rouge, on inverse
                {
                    x = 300 - x;
                }
            }

            // On randomize un peu
            short dx, dy;
            int precision = precisionOfTourelle[adrVise];
            dx = ((rand()%precision) - precision/2)/10;
            dy = ((rand()%precision) - precision/2)/10;
            x += dx;
            y += dy;


            // On envoie
            unsigned char trames[4];
            for(unsigned char i=0; i<4; i++)
                trames[i] = 0; // L'adresse d'envoi est le Psoc

            trames[0] += 0b00011111& (x >> 5);
            trames[1] += 0b00011111& ((x << 5)>>5);
            trames[2] += 0b00011111& (y >> 5);
            trames[3] += 0b00011111& ((y << 5)>>5);

            for(unsigned char i; i<4; i++)
                sendMessageTourelleToSoc(trames[i], 0);
        }
    }
}

void ModuleTourelle::envoyerUnMessage(unsigned char message)
{
    traiterMessageTourelle(message);
}

void sendMessageTourelle(unsigned char c)
{
    moduleTourelle->ajouterMessageRecu(c);
}

void forcerTourelle()
{
    moduleTourelle->run(0);
}

void sendMessageTourelleToSoc(unsigned char c, float dt)
{
    moduleTourelle->ajouterMessageAEnvoyer(c, dt);
}

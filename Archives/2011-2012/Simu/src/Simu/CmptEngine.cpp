#include "CmptEngine.h"

using namespace Simu;

CmptEngine::CmptEngine() :
IEngine(FREQ_CMPT)
{
    reinit();
}

CmptEngine::~CmptEngine()
{
    traiterMessages(); // Supprime les messages
}

void CmptEngine::run(float dt)
{
    traiterMessages();

    if(!finMatch)
    {
        // Timer
        lastRetirerSeconde += dt;
        if(lastRetirerSeconde >= 1)
        {
            lastRetirerSeconde -= 1;
            changed |= retirerSeconde();
        }

        // Points
        if(debutMatch)
        {
            askPoints();
        }

        if(changed)
            sendChangesToGraphicEngine();
    }
}

void CmptEngine::reinit()
{
    changed = true;
    debutMatch = (SECONDES_PREMATCH == 0);
    finMatch = false;
    pointsViolet = 0;
    pointsRouge = 0;
    lastRetirerSeconde = 0;
    if(debutMatch)
    {
        minutes = DUREE_MATCH_MINUTES;
        secondes = DUREE_MATCH_SECONDES;
    }
    else
    {
        minutes = 0;
        secondes = SECONDES_PREMATCH;
    }
}

bool CmptEngine::retirerSeconde()
{
    if(!finMatch)
    {
        if(!debutMatch)
        {
            if(secondes == 0)
            {
                secondes = DUREE_MATCH_SECONDES;
                minutes = DUREE_MATCH_MINUTES;
                debutMatch = true;
            }
            else
            {
                secondes --;
            }
        }
        else
        {
            if(secondes == 0)
            {
                secondes = 59;
                minutes--;
            }
            else
            {
                secondes--;
            }

            finMatch = (secondes == 0 && minutes == 0);
        }
        return true;
    }

    return false;
}

void CmptEngine::askPoints() // Appelle le moteur physique
{
    sendMessage(adresse.physicEngine, MSG_CALC_PT, NULL, true);
}

void CmptEngine::sendChangesToGraphicEngine() // Appelle le moteur graphique
{
    /// TODO
    /*
    if(!debutMatch)
    {
        printf("%d\n", secondes);
    }
    else
    {
        printf("%d:%2d   %d - %d\n", minutes, secondes, pointsRouge, pointsViolet);
    }
    */

    changed = false;
}

void CmptEngine::traiterMessages()
{
    Message m;
    while(!messages.empty())
    {
        m = messages.back();
        messages.pop_back();
        switch(m.type)
        {
            case MSG_PT_ROUGE :
            {

                int* data = (int*)m.data;
                changed |= (pointsRouge != *data);
                pointsRouge = *data;
                delete data;
                break;
            }

            case MSG_PT_VIOLET :
            {
                int* data = (int*)m.data;
                changed |= (pointsViolet != *data);
                pointsViolet = *data;
                delete data;
                break;
            }

            default : break;
        }
    }
}

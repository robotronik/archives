#include "ModuleAsser.h"

using namespace Simu;

ModuleAsser* moduleAsser = NULL;

ModuleAsser::ModuleAsser() : Module()
{
    //ctor
    moduleAsser = this;
}


void ModuleAsser::run(float dt)
{
    int nbExecutions = (int)(dt*FREQ_ASSER);
    for(int i=0; i<nbExecutions; i++)
    {
        t += dt;
        traiterMessagesRecus();
        asser();
        // On place le nouveau x,y,alpha
        updateXYAlpha(dt/((float)nbExecutions));
        envoyerMessages();
    }
}

void ModuleAsser::traiterUnMessage(unsigned char message)
{
    message_processing(message);
    if(message == TEST)
    {
        printf("Asser a envoye message TEST\n");
    }
}

void ModuleAsser::envoyerUnMessage(unsigned char message)
{
    traiterMessageAsser(message);
}

void ModuleAsser::updateXYAlpha(float dt)
{
    unsigned int trueX = posX.all;
    if(false)//couleurRobot() != 0) // Violet
        trueX = 0xFFFFFFFF - trueX;
    trueX = (((unsigned long long)posX.all)*3000)/0xFFFFFFFF;
    unsigned int trueY = 2000-((((unsigned long long)posY.all)*3000)/0xFFFFFFFF);


    int trueAlpha;
    /*
    // Conversion -> PSoc
    if(couleurRobot() == 0) // Rouge
        trueAlpha = TOUR-posAlpha;
    else
        trueAlpha = (posAlpha+(TOUR>>1))%TOUR; // La mm dans les deux sens
    */
    // Conversion
	trueAlpha = (unsigned long) posAlpha*360/TOUR;
    if(true)//couleurRobot() != 0) // Violet
    	trueAlpha = (360+90-trueAlpha)%360;

    //printf("Y : %d\n", trueY);

    irr::core::vector3df curPos(((float)trueX)*0.01f - 15.0f, 0, 10.0f - ((float)trueY)*0.01f);
    irr::core::vector3df curRot(0,(float)trueAlpha,0);

    //printf("Alpha : %d\n", trueAlpha);

    // On replace le robot a cette position
    data->getRobotViolet().updatePosRot(curPos, curRot, dt);
}

void ModuleAsser::forcer()
{
    traiterMessagesRecus();
    asser();
    envoyerMessages();
}

void forcerAsser()
{
    moduleAsser->forcer();
}

void sendMessageAsser(unsigned char c)
{
    moduleAsser->ajouterMessageRecu(c);
}

void sendMessageAsserToPsoc(unsigned char c, float dt)
{
    moduleAsser->ajouterMessageAEnvoyer(c, dt);
}

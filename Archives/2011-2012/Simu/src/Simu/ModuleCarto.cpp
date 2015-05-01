#include "ModuleCarto.h"

using namespace Simu;

ModuleCarto* moduleCarto = NULL;

ModuleCarto::ModuleCarto() : Module()
{
    moduleCarto = this;
    init_main();
}

void ModuleCarto::run(float dt)
{

    t += dt;

    traiterMessagesRecus();
    boucle();
    envoyerMessages();
}

void ModuleCarto::traiterUnMessage(unsigned char message)
{
    message_processing_carto(message);
}

void ModuleCarto::envoyerUnMessage(unsigned char message)
{
    traiterMessageCarto(message);
}

void forcerCarto()
{
    moduleCarto->run(0);
}

void sendMessageCarto(unsigned char c)
{
    moduleCarto->ajouterMessageRecu(c);
}

void sendMessageCartoToSoc(unsigned char c, float dt)
{
    moduleCarto->ajouterMessageAEnvoyer(c, dt);
}

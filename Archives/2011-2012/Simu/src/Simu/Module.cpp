#include "Module.h"

using namespace Simu;

Module::Module() : IEngine(), t(0), messagesRecus(), messagesEnvoyes()
{
    //ctor
}

Module::~Module()
{
}

void Module::afficherBoite()
{
    printf("Recus : ");
        messagesRecus.afficher();
    printf("A Envoyer : ");
        messagesEnvoyes.afficher();
}


void Module::traiterMessagesRecus()
{
    MessageUart* it;
    while(!messagesRecus.isEmpty())
    {
        it = messagesRecus.pop();
        traiterUnMessage(it->message);
        delete it;
    }

}

void Module::envoyerMessages()
{
    MessageUart* it;
    FileMessageUart unDone;
    while(!messagesEnvoyes.isEmpty())
    {
        it = messagesEnvoyes.pop();
        if(it->t <= t)  /// CHANGED
        {
            envoyerUnMessage(it->message);
            delete it;
        }
        else
            unDone.push(it);
    }
    messagesEnvoyes = unDone;
    unDone.falseEmpty();
}

void Module::ajouterMessageRecu(unsigned char message)
{
    MessageUart* nouveau = new MessageUart(message);
    messagesRecus.push(nouveau);
}

void Module::ajouterMessageAEnvoyer(unsigned char message, float dt)
{
    MessageUart* nouveau = new MessageUart(message, t+dt);
    messagesEnvoyes.push(nouveau);
}

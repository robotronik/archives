#ifndef MODULE_H
#define MODULE_H

#include "../Simu/uart.h"
#include "../Simu/IEngine.h"

namespace Simu
{

class Module : public IEngine
{
    public:
        Module();
        virtual ~Module();

        void ajouterMessageRecu(unsigned char message);
        void ajouterMessageAEnvoyer(unsigned char message, float dt);

        void traiterMessagesRecus();
        void envoyerMessages();

        void afficherBoite();

    protected:

        virtual void traiterUnMessage(unsigned char message) = 0;
        virtual void envoyerUnMessage(unsigned char message) = 0;

        float t;
    private :
        FileMessageUart messagesRecus;
        FileMessageUart messagesEnvoyes;
};

}

#endif // MODULE_H

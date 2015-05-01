#ifndef MODULECARTO_H_INCLUDED
#define MODULECARTO_H_INCLUDED

#include "Module.h"
#include "../Carto/help_main.h"
#include "Data.h"

// Le seul extern acceptable est le traitement d'IT Psoc
extern void traiterMessageCarto(unsigned char message);

/*  Interfaces cartes  */
// Appellee par le Psoc
void forcerCarto();
void sendMessageCarto(unsigned char c);
// Appellee par la carto
void sendMessageCartoToSoc(unsigned char c, float dt);


namespace Simu
{
class ModuleCarto : public Module
{
    public:
        ModuleCarto();

        virtual void run(float dt);  // dt en secondes


    protected :
        void traiterUnMessage(unsigned char message);
        void envoyerUnMessage(unsigned char message);
};

}


#endif // MODULECARTO_H_INCLUDED

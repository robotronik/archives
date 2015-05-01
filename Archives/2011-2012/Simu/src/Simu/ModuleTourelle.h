#ifndef MODULETOURELLE_H
#define MODULETOURELLE_H

#include "Module.h"
#include "Data.h"
#include "../Soc/cst.h"

// Le seul extern acceptable est le traitement d'IT Psoc
extern void traiterMessageTourelle(unsigned char message);
extern unsigned char couleurRobotI;

/*  Interfaces cartes  */
// Appellee par le Psoc
void sendMessageTourelle(unsigned char c);
void forcerTourelle();
// Appellee par la tourelle
void sendMessageTourelleToSoc(unsigned char c, float dt);


namespace Simu
{
class ModuleTourelle : public Module
{
    public:
        ModuleTourelle();

        virtual void run(float dt);  // dt en secondes


    protected :
        void traiterUnMessage(unsigned char message);
        void envoyerUnMessage(unsigned char message);

    private :
        unsigned int precisionOfTourelle[nombreTourelles];
};
}

#endif // MODULETOURELLE_H

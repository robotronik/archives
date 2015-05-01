#ifndef MODULEASSER_H
#define MODULEASSER_H

#include "Module.h"
#include "Data.h"
#include "../Asser/main.h"
#include "../Asser/asser.h"
#include "../Asser/message.h"
#include "../Asser/prog2.h"

#define PRECISION (long)0xFFFF*1/3000

// Le seul extern acceptable est le traitement d'IT Psoc
extern void traiterMessageAsser(unsigned char m);
/*  Interfaces cartes  */
// Appellee par le Psoc
void forcerAsser();
void sendMessageAsser(unsigned char c);
// Appellee par l'asser
void sendMessageAsserToPsoc(unsigned char c, float dt);

namespace Simu
{

class ModuleAsser : public Module
{
    public:
        ModuleAsser();

        virtual void run(float dt);  // dt en secondes
        void forcer();

    protected :
        void traiterUnMessage(unsigned char message);
        void envoyerUnMessage(unsigned char message);

    private :
        void updateXYAlpha(float dt);
};

}


#endif // MODULEASSER_H

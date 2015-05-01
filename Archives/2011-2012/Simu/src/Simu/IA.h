#ifndef IA_H
#define IA_H

#include "ModuleCarto.h"
#include "ModuleAsser.h"
#include "ModuleTourelle.h"
#include "../Soc/machine.h"
#include "../Soc/recTourelle.h"
#include <ctime>

#define NB_MODULES 3

namespace Simu
{

enum NomModule
{
    SOC, ESPION, CARTO
};

// Structure de file
struct Message_Uart
{
    NomModule emetteur;
    NomModule recepteur;
    char value;
    Message_Uart* suivant;
};

class IA : public IEngine
{
    public:
        IA();
        ~IA();

        void run(float dt);

    private:
        Module* modules[NB_MODULES];
        float t;
};

}

#endif // IA_H

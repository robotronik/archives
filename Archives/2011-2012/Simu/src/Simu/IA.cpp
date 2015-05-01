#include "IA.h"

using namespace Simu;

IA::IA() : IEngine(FREQ_IA), t(0)
{
    srand(time(NULL));
    //ctor
    modules[0] = new ModuleAsser;
    modules[1] = new ModuleCarto;
    modules[2] = new ModuleTourelle;
    //ModuleAsser* asser = (ModuleAsser*) modules[0];
    //asser->receiveCommandeXY(1500,1800);

    initPssoc();
}

IA::~IA()
{
    for(int i=0; i<NB_MODULES; i++)
        delete modules[i];
}

void IA::run(float dt)
{
    t += dt;

    // Affichage DEBUG
   // printf("Asser\n");
   // modules[0]->afficherBoite();

    for(int i=0; i< NB_MODULES; i++)
        modules[i]->run(dt);
    runMachine();
}

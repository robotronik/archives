#include "PhysicEngine.h"

using namespace Simu;

PhysicEngine::PhysicEngine() :
IEngine(FREQ_PHYS),
nWorld(NewtonCreate())
{
    irr::core::vector3df min(-100,-100,-100), max(100,100,100);
    NewtonSetWorldSize (nWorld, &min.X, &max.X);

}

PhysicEngine::~PhysicEngine()
{
    NewtonDestroy(nWorld);
}

void PhysicEngine::run(float dt)
{
    traiterMessage();
    NewtonUpdate(nWorld, dt);
}

void PhysicEngine::traiterMessage()
{
    Message m;
    while(!messages.empty())
    {
        m = messages.back();
        messages.pop_back();
        switch(m.type)
        {
            case MSG_CALC_PT :
            {
                // Calcul des points
                /// TODO
                int* ptRouge = new int;
                int* ptViolet = new int;
                *ptRouge = 0;
                *ptViolet = 0;
                sendMessage(adresse.cmptEngine, MSG_PT_ROUGE, ptRouge, false);
                sendMessage(adresse.cmptEngine, MSG_PT_VIOLET, ptViolet, false);
                break;
            }

            default : break;
        }
    }
}

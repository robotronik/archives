#ifndef PHYSICENGINE_H
#define PHYSICENGINE_H

#include "IEngine.h"

#include <Newton/Newton.h>

namespace Simu
{

class PhysicEngine : public IEngine
{
    public:
        PhysicEngine();
        ~PhysicEngine();
        NewtonWorld* getWorld() const { return nWorld; }

        void run(float dt);

    private:
        NewtonWorld* nWorld;

        void traiterMessage();
};

}
#endif // PHYSICENGINE_H

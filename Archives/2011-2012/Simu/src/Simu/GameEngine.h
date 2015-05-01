#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "GraphicEngine.h"
#include "JoysEngine.h" // To Change
#include "PhysicEngine.h"
#include "CmptEngine.h"
#include "IA.h"

namespace Simu
{

class GameEngine : public IEngine
{
    public:
        GameEngine();
        ~GameEngine();

        void exec();
        void run(float dt);

        static GameEngine* getInstance() {return instance;}
        GraphicEngine& getGE()  {return graphicEngine;}

    private:
        GraphicEngine graphicEngine;
        PhysicEngine physicEngine;
        JoysEngine joysEngine;
        CmptEngine cmptEngine;
        IA ia;

        float previousTime;

        bool continuer;

        static GameEngine* instance;
};

}

#endif // GAMEENGINE_H

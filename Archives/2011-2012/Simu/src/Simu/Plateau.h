#ifndef PLATEAU_H
#define PLATEAU_H

#include "IObject.h"
#include <cstdio>
#include <irr/irrlicht.h>

#define NB_COL 23 // Nombre de collisions a ajouter

namespace Simu
{

class Plateau : public IObject
{
    public:
        Plateau(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld);

    private:
        NewtonCollision* creerCollision(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld);
};


}

#endif // PLATEAU_H

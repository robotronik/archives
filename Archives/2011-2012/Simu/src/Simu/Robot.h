#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include "IObject.h"

#define MASSE_ROBOT 50 // 50 kg
#define MASSE_PINCE 50.0f
#define HAUTEUR_ROBOT 0.1f

namespace Simu
{

void RobotTransformCallback(const NewtonBody * body, const float* mat, int thread);

class Pince : public IObject
{
    public :
        Pince(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld, bool gauche, irr::core::vector3df _pos, irr::core::vector3df _rot);

};

class Robot : public IObject
{
    public :
        Robot(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld, irr::core::vector3df _pos, irr::core::vector3df _rot);

        virtual void move(irr::core::vector3df du, irr::core::vector3df dr, float dt, const IObject& decor);
        virtual void move(irr::core::vector3df vit, float alpha, float dt, const IObject& decor);
        virtual void updatePosRot(irr::core::vector3df pos, irr::core::vector3df rot, float dt);
        void updatePince();

        Pince& getPinceGauche() {return pinceGauche;}
        Pince& getPinceDroite() {return pinceDroite;}

    private :
        Pince pinceGauche;
        Pince pinceDroite;
    friend void RobotTransformCallback(const NewtonBody * body, const float* mat, int thread);
};


}

#endif // ROBOT_H_INCLUDED

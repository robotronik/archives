#include "Robot.h"

using namespace Simu;

Pince::Pince(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld, bool gauche, irr::core::vector3df _pos, irr::core::vector3df _rot) :
IObject( (gauche) ? sceneManager->addMeshSceneNode(sceneManager->getMesh("Model/pince_gauche.obj")) :
                    sceneManager->addMeshSceneNode(sceneManager->getMesh("Model/pince_droite.obj")),
        MASSE_ROBOT, true)
{
    irr::core::matrix4 mat;
    mat.makeIdentity();
    mat.setTranslation(_pos);
    mat.setRotationDegrees(_rot);
    setPosRot(mat);

    // Newton
    NewtonCollision* collision = NULL;
    if(gauche)
        collision = createCollisionFromMesh(nWorld, sceneManager->getMesh("Model/pince_gauche.obj"));
    else
        collision = createCollisionFromMesh(nWorld, sceneManager->getMesh("Model/pince_droite.obj"));
    body = NewtonCreateBody (nWorld, collision, mat.pointer());
    NewtonBodySetUserData(body, this);

    // On affecte une masse
    irr::core::vector3df inertie;
    irr::core::vector3df G(0,
                           2.025,
                           0);
    NewtonConvexCollisionCalculateInertialMatrix(collision, &inertie.X, &G.X);
    inertie *= MASSE_PINCE;

    // On définit ensuite la masse et l'inertie pour ce corps
    NewtonBodySetMassMatrix (body, MASSE_PINCE, inertie.X, inertie.Y, inertie.Z);


    NewtonBodySetForceAndTorqueCallback (body, ApplyForceAndTorqueCallbackWithoutGravity);
    NewtonBodySetTransformCallback(body, TransformCallback);


    NewtonReleaseCollision (nWorld, collision);
}

Robot::Robot(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld, irr::core::vector3df _pos, irr::core::vector3df _rot) :
IObject(sceneManager->addMeshSceneNode(sceneManager->getMesh("Model/mainrobot.obj")) ,MASSE_ROBOT, true),
pinceGauche(sceneManager, nWorld, true, _pos, _rot),
pinceDroite(sceneManager, nWorld, false, _pos, _rot)
{
    irr::core::matrix4 mat;
    mat.makeIdentity();
    mat.setTranslation(_pos);
    mat.setRotationDegrees(_rot);
    setPosRot(mat);

    // Newton
    NewtonCollision* collision = createCollisionFromMesh(nWorld, sceneManager->getMesh("Model/mainrobot.obj"));
    body = NewtonCreateBody (nWorld, collision, mat.pointer());
    NewtonBodySetUserData(body, this);

    // Hinge
    /*
        1.071
        1.151
        1.767 -> axe
    */

    // On calcul l'inertie du corps
    irr::core::vector3df inertie;
    irr::core::vector3df G(0,
                           2.025,
                           0);
    NewtonConvexCollisionCalculateInertialMatrix(collision, &inertie.X, &G.X);
    NewtonReleaseCollision (nWorld, collision);
    inertie *= MASSE_ROBOT;

    // On définit ensuite la masse et l'inertie pour ce corps
    NewtonBodySetMassMatrix (body, MASSE_ROBOT, inertie.X, inertie.Y, inertie.Z);

    // On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
    NewtonBodySetForceAndTorqueCallback (body, ApplyForceAndTorqueCallback);
    NewtonBodySetTransformCallback(body, RobotTransformCallback);
}

void Robot::move(irr::core::vector3df du, irr::core::vector3df dr, float dt, const IObject& decor)
{
    IObject::move(du, dr, dt, decor);
    // Bouger les pinces
    updatePince();
}

void Robot::move(irr::core::vector3df vit, float alpha, float dt, const IObject& decor)
{
    // C'est cette fonction qui est appellee
    IObject::move(vit, alpha, dt, decor);
    // Bouger les pinces
    updatePince();
}

void Robot::updatePosRot(irr::core::vector3df _pos, irr::core::vector3df _rot, float dt)
{
    irr::core::vector3df vit = irr::core::vector3df(0,0,0);//2.0f*(_pos - pos)/dt;
    irr::core::vector3df omega = irr::core::vector3df(0,0,0);//(_rot - rot)/dt;

    setPosRot(_pos, _rot, vit, omega);

    updatePince();
}

void Robot::updatePince()
{
    irr::core::vector3df vit, omega;
    NewtonBodyGetVelocity(body, &vit.X);
    NewtonBodyGetOmega(body, &omega.X);
    pinceGauche.setPosRot(getPos(), getRot(), vit, omega);
    pinceDroite.setPosRot(getPos(), getRot(), vit, omega);
}

namespace Simu
{

void RobotTransformCallback(const NewtonBody * body, const float* mat, int thread)
{
    Robot* object = (Robot*) NewtonBodyGetUserData(body);
    irr::core::matrix4 matIrr;
    matIrr.setM(mat);
    object->setPosRot(matIrr);

    if(object->isControlled)
        object->resetVelOme();
    else
    {
        // on limite la vitesse
        irr::core::vector3df vit;
        NewtonBodyGetVelocity(body, &vit.X);
        float length = vit.getLength();
        if(length > 10.0f)
        {
            vit.setLength(10.0f);
            NewtonBodySetVelocity(body, &vit.X);
        }
    }
    object->updatePince();
}


}

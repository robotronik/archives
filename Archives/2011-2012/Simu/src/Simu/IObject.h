#ifndef IOBJECT_H_INCLUDED
#define IOBJECT_H_INCLUDED

#include <Newton/Newton.h>
#include <irr/irrlicht.h>

#define GRAVITY 98.1f


namespace Simu
{

void ApplyForceAndTorqueCallback (const NewtonBody * body, float timestep, int thread);
void ApplyForceAndTorqueCallbackWithoutGravity(const NewtonBody * body, float timestep, int thread);
void TransformCallback(const NewtonBody * body, const float* mat, int thread);

NewtonCollision *createCollisionFromMesh(NewtonWorld* nWorld, irr::scene::IMesh *irr_mesh);

class IObject
{

    public :
        IObject(irr::scene::ISceneNode* _mesh, float _masse, bool controlled = false);
        virtual ~IObject();

        virtual void move(irr::core::vector3df du, irr::core::vector3df dr, float dt, const IObject& decor); // relatifs a l'objet
        virtual void move(irr::core::vector3df vit, float alpha, float dt, const IObject& decor); // relatifs a l'objet
        void setPosRot(irr::core::vector3df pos, irr::core::vector3df rot, irr::core::vector3df vit, irr::core::vector3df omega);
        NewtonBody* getBody() const {return body;}

        irr::core::vector3df getPos() const {return pos;}
        irr::core::vector3df getRot() const {return rot;}
        irr::core::matrix4 getMatrix() const;


    protected :
        NewtonBody * body;
        irr::scene::ISceneNode* mesh;
        void setPosRot(const irr::core::matrix4& matIrr); // Synchro Irrlicht
        irr::core::vector3df pos, rot;
        void resetVelOme();
        bool isControlled;

    private :
        bool collideWith(const IObject& obj, const irr::core::matrix4& mat);
        float masse; // Donne egalement l'information de statisme (masse == 0)

    friend void ApplyForceAndTorqueCallback (const NewtonBody * body, float timestep, int thread);
    friend void TransformCallback(const NewtonBody * body, const float* mat, int thread);
};

}

#endif // IOBJECT_H_INCLUDED

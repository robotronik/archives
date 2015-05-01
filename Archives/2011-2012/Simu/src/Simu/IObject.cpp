#include "IObject.h"

using namespace Simu;

IObject::IObject(irr::scene::ISceneNode* _mesh, float _masse, bool controlled) :
body(NULL),
mesh(_mesh),
masse(_masse),
isControlled(controlled)
{
}

IObject::~IObject()
{

}

irr::core::matrix4 IObject::getMatrix() const
{
    irr::core::matrix4 mat;
    NewtonBodyGetMatrix(body, mat.pointer());
    return mat;
}

void IObject::move(irr::core::vector3df du, irr::core::vector3df dr, float dt, const IObject& decor)
{
    irr::core::matrix4 mat, t, r;
    NewtonBodyGetMatrix(body, mat.pointer());
    t.makeIdentity(); r.makeIdentity();
    t.setTranslation(du);
    r.setRotationDegrees(dr);
    mat = mat*t*r;

    if(!collideWith(decor, mat))
    {
        NewtonBodySetMatrix(body, t.pointer());

        setPosRot(mat);

        irr::core::vector3df velocity = du / dt;
        irr::core::vector3df omega = dr / dt;
        NewtonBodySetVelocity(body, &velocity.X);
        NewtonBodySetOmega(body, &omega.X);
    }
}

void IObject::move(irr::core::vector3df vit, float alpha, float dt, const IObject& decor)
{
    irr::core::vector3df du = vit * dt;
    irr::core::vector3df dr(0, alpha, 0);

    irr::core::matrix4 mat, t, r;
    NewtonBodyGetMatrix(body, mat.pointer());
    mat.rotateVect(vit);

    t.makeIdentity(); r.makeIdentity();
    t.setTranslation(du);
    r.setRotationDegrees(dr);
    mat = mat*t*r;


    if(!collideWith(decor, mat))
    {
        NewtonBodySetMatrix(body, mat.pointer());

        setPosRot(mat);

        irr::core::vector3df omega = (dr/dt) * 0.01f;
        vit *= 2.0f;
        NewtonBodySetVelocity(body, &vit.X);
        NewtonBodySetOmega(body, &omega.X);
    }
}

void IObject::resetVelOme()
{
    irr::core::vector3df u(0,0,0);
    NewtonBodySetVelocity(body, &u.X);
    NewtonBodySetOmega(body, &u.X);
}

bool IObject::collideWith(const IObject& obj, const irr::core::matrix4& mat)
{
    //contacts, normals, penetrations, threadindex
    float contacts[20*3], normals[20*3], penetrations[20*3];
    for(int i=0; i<20*3; i++)
    {
        contacts[i] = 0;
        normals[i] = 0;
        penetrations[i] = 0;
    }
    NewtonWorld* nWorld = NewtonBodyGetWorld(body);
    irr::core::matrix4 tMat = mat, t;
    t.makeIdentity();
    t.setTranslation(irr::core::vector3df(0,0.1,0));
    tMat *= t;

    int nbContacts = NewtonCollisionCollide(nWorld, 20, NewtonBodyGetCollision(body), tMat.pointer(), NewtonBodyGetCollision(obj.body), obj.getMatrix().pointer(),
               contacts, normals, penetrations, 0);
    return (nbContacts > 0);
}

void IObject::setPosRot(const irr::core::matrix4& matIrr)
{
    pos = matIrr.getTranslation();
    rot = matIrr.getRotationDegrees();
    mesh->setPosition(pos);
    mesh->setRotation(rot);
}

void IObject::setPosRot(irr::core::vector3df pos, irr::core::vector3df rot, irr::core::vector3df vit, irr::core::vector3df omega)
{
    this->pos = pos;
    this->rot = rot;
    // Irrlicht
    mesh->setPosition(pos);
    mesh->setRotation(rot);

    // Newton
    irr::core::matrix4 mat;
    mat.makeIdentity();
    mat.setTranslation(pos);
    mat.setRotationDegrees(rot);
    NewtonBodySetMatrix(body, mat.pointer());
    NewtonBodySetVelocity(body, &vit.X);
    NewtonBodySetOmega(body, &omega.X);
}

namespace Simu
{

void ApplyForceAndTorqueCallback (const NewtonBody * body, float timestep, int thread)
{
   // On récupère en premier lieu la masse ainsi que l'inertie
   float masse; // Contiendra la masse de l'objet pris en paramètre par la fonction
   irr::core::vector3df inertie; // Contiendra l'inertie du corps
   irr::core::vector3df force; // Spécifiera la force appliquée sur le corps

   NewtonBodyGetMassMatrix (body, &masse, &inertie.X, &inertie.Y, &inertie.Z);

   force.X = 0.0f;
   force.Y = -masse * GRAVITY;
   force.Z = 0.0f;

   NewtonBodyAddForce (body, &force.X); // On ajoute la force au corps
}

void ApplyForceAndTorqueCallbackWithoutGravity(const NewtonBody * body, float timestep, int thread)
{
   // On récupère en premier lieu la masse ainsi que l'inertie
   /*
   float masse; // Contiendra la masse de l'objet pris en paramètre par la fonction
   irr::core::vector3df inertie; // Contiendra l'inertie du corps
   irr::core::vector3df force; // Spécifiera la force appliquée sur le corps

   NewtonBodyGetMassMatrix (body, &masse, &inertie.X, &inertie.Y, &inertie.Z);

   force.X = 0.0f;
   force.Y = 0.0f;
   force.Z = 0.0f;

   NewtonBodyAddForce (body, &force.X); // On ajoute la force au corps
    */
   // NewtonBodySetFreezeState(body, 1);
}

void TransformCallback(const NewtonBody * body, const float* mat, int thread)
{
    IObject* object = (IObject*) NewtonBodyGetUserData(body);
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
}

NewtonCollision *createCollisionFromMesh(NewtonWorld* nWorld, irr::scene::IMesh *irr_mesh)
{
    unsigned int nMeshBuffer = 0; //Mesh Buffer count
    irr::scene::IMeshBuffer *mesh_buffer = NULL;
    float *vertices; //Array to store vertices

    irr::u32 nVertices = 0;

    //Get number of vertices
    for( nMeshBuffer=0 ; nMeshBuffer < irr_mesh->getMeshBufferCount() ; nMeshBuffer++ )
    {
        nVertices += irr_mesh->getMeshBuffer(nMeshBuffer)->getVertexCount();
    }

    //create buffer for vertices
    vertices = new float[nVertices * 3];
    irr::u32 tmpCounter = 0;

    //Get (irr_)mesh buffers and copy face vertices
    for( nMeshBuffer=0 ; nMeshBuffer < irr_mesh->getMeshBufferCount() ; nMeshBuffer++ )
    {
        mesh_buffer = irr_mesh->getMeshBuffer(nMeshBuffer);
        //Get pointer to vertices and indices
        irr::video::S3DVertex* S3vertices = (irr::video::S3DVertex*)mesh_buffer->getVertices();
        //copy vertices from mesh to buffer
        for(unsigned int i=0; i<mesh_buffer->getVertexCount(); i++)
        {
            vertices[tmpCounter++] = S3vertices[i].Pos.X;
            vertices[tmpCounter++] = S3vertices[i].Pos.Y;
            vertices[tmpCounter++] = S3vertices[i].Pos.Z;
        }

    }

    //Create Newton collision object
    NewtonCollision *collision_obj = NewtonCreateConvexHull(nWorld,nVertices,vertices,sizeof(float)*3, 0.001f, -1,NULL);

    //delete vertices
    delete [] vertices;
    return collision_obj;
}

}

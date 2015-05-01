#include "Plateau.h"

using namespace Simu;

Plateau::Plateau(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld) :
IObject(sceneManager->addMeshSceneNode(sceneManager->getMesh("Model/plateau.obj")),0)
{
    // Irrlicht
    mesh->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    mesh->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
    mesh->setMaterialFlag(irr::video::EMF_COLOR_MATERIAL, true);

    // Newton
    irr::core::matrix4 mat;
    mat.makeIdentity();
    NewtonCollision* collision = creerCollision(sceneManager, nWorld);
    body = NewtonCreateBody (nWorld, collision, mat.pointer());
    NewtonReleaseCollision (nWorld, collision);

    // Le bas
    mat.setTranslation(irr::core::vector3df(0,-5,0));
    collision = NewtonCreateBox(nWorld, 60, 10, 40, -1, NULL);
    NewtonCreateBody (nWorld, collision, mat.pointer());
    NewtonReleaseCollision (nWorld, collision);
}

NewtonCollision* Plateau::creerCollision(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld)
{
    NewtonCollision* collision = NewtonCreateTreeCollision (nWorld, 0);
    irr::scene::IMeshSceneNode* objCol;
    char nomFichier[30];

    NewtonTreeCollisionBeginBuild (collision);
    for(int i=1; i<= NB_COL; i++)
    {
        sprintf(nomFichier, "Model/col/plateau_col%d.obj", i);
        objCol = sceneManager->addMeshSceneNode(sceneManager->getMesh(nomFichier));

        unsigned int cMeshBuffer, j;
        int v1i, v2i, v3i;
        irr::scene::IMeshBuffer *mb;

        float vArray[9]; // vertex array (3*3 floats)

        for (cMeshBuffer=0; cMeshBuffer < objCol->getMesh()->getMeshBufferCount(); cMeshBuffer++)
        {
            mb = objCol->getMesh()->getMeshBuffer(cMeshBuffer);

            irr::video::S3DVertex* mb_vertices = (irr::video::S3DVertex*)mb->getVertices();

            irr::u16* mb_indices  = mb->getIndices();

            // add each triangle from the mesh
            for (j=0; j < mb->getIndexCount(); j+=3)
            {
                v1i = mb_indices[j];
                v2i = mb_indices[j+1];
                v3i = mb_indices[j+2];

                vArray[0] = mb_vertices[v1i].Pos.X;
                vArray[1] = mb_vertices[v1i].Pos.Y;
                vArray[2] = mb_vertices[v1i].Pos.Z;
                vArray[3] = mb_vertices[v2i].Pos.X;
                vArray[4] = mb_vertices[v2i].Pos.Y;
                vArray[5] = mb_vertices[v2i].Pos.Z;
                vArray[6] = mb_vertices[v3i].Pos.X;
                vArray[7] = mb_vertices[v3i].Pos.Y;
                vArray[8] = mb_vertices[v3i].Pos.Z;

                NewtonTreeCollisionAddFace(collision, 3, vArray, 3*sizeof(float), 1);
            }

        }

    }

    NewtonTreeCollisionEndBuild (collision, 0); // 1 = optimisation

    return collision;
}

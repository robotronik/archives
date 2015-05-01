#include "Jeton.h"

using namespace Simu;

Jeton::Jeton(NewtonWorld* nWorld, irr::scene::ISceneManager* sceneManager, irr::scene::IMeshSceneNode* _mesh, TokenColor couleur, irr::core::vector3df _position, irr::core::vector3df _rotation) :
IObject(_mesh, getMasse(couleur)), ctype(couleur)
{
    mesh = _mesh->clone();
    mesh->setVisible(true);

    // Newton
    irr::core::matrix4 mat;
    mat.makeIdentity();
    mat.setTranslation(_position);
    mat.setRotationDegrees(_rotation);
    setPosRot(mat);
    NewtonCollision* collision = creerEnveloppeComplexe(nWorld, sceneManager, ctype, _position, _rotation);
    body = NewtonCreateBody (nWorld, collision, mat.pointer());
    NewtonBodySetUserData(body, this);

    // On calcul l'inertie du corps
    irr::core::vector3df inertie;
    irr::core::vector3df G(0,
                           (ctype == LINGOT)? 0.243f : 0.118f,
                           0);
    NewtonConvexCollisionCalculateInertialMatrix(collision, &inertie.X, &G.X);
    NewtonReleaseCollision (nWorld, collision);
    inertie *= getMasse(couleur);

    // On définit ensuite la masse et l'inertie pour ce corps
    NewtonBodySetMassMatrix (body,getMasse(couleur), inertie.X, inertie.Y, inertie.Z);

    // On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
    NewtonBodySetForceAndTorqueCallback (body, ApplyForceAndTorqueCallback);
    NewtonBodySetTransformCallback(body, TransformCallback);
}

int Jeton::getPoints()
{
    switch(ctype)
    {
        case BLANC : return 1;
        case NOIR : return 0;
        case LINGOT : return 3;
    }
    return 0;
}

float Jeton::getMasse(TokenColor ctype)
{
    switch(ctype)
    {
        case BLANC : return 0.02f;
        case NOIR : return 0.02f;
        case LINGOT : return 1.0f;
    }
    return 0;
}

NewtonCollision* Jeton::creerEnveloppeComplexe(NewtonWorld* nWorld, irr::scene::ISceneManager* sceneManager, TokenColor couleur, irr::core::vector3df _position, irr::core::vector3df _rotation)
{
    NewtonCollision* collision = NULL;
    if(couleur != LINGOT)
    {
        NewtonCollision* blocs[2];
        irr::scene::IMeshSceneNode* bloc1 = sceneManager->addMeshSceneNode (sceneManager->getMesh ("Model/col/jeton_col1.obj"));
        irr::scene::IMeshSceneNode* bloc2 = sceneManager->addMeshSceneNode (sceneManager->getMesh ("Model/col/jeton_col2.obj"));
        bloc1->setPosition(_position);
        bloc2->setPosition(_position);
        bloc1->setRotation(_rotation);
        bloc1->setRotation(_rotation);
        bloc2->setRotation(_rotation);
        blocs[0] = createCollisionFromMesh(nWorld, bloc1->getMesh());
        blocs[1] = createCollisionFromMesh(nWorld, bloc2->getMesh());

        collision = NewtonCreateCompoundCollision (nWorld, 2, blocs, -1);

        NewtonReleaseCollision (nWorld, blocs[0]);
        NewtonReleaseCollision (nWorld, blocs[1]);
        bloc1->remove();
        bloc2->remove();
    }
    else
    {
        irr::scene::IMeshSceneNode* meshLingot = sceneManager->addMeshSceneNode (sceneManager->getMesh ("Model/lingot.obj"));
        collision = createCollisionFromMesh(nWorld, meshLingot->getMesh());
        meshLingot->remove();
    }

    return collision;
}

Tresor::Tresor(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld)
{
    // On commence par charger les modeles
    lingot = sceneManager->addMeshSceneNode (sceneManager->getMesh ("Model/lingot.obj"));
        lingot->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        lingot->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
        lingot->setMaterialFlag(irr::video::EMF_COLOR_MATERIAL, true);
        lingot->setVisible(false);
    jetonBlanc = sceneManager->addMeshSceneNode (sceneManager->getMesh ("Model/jeton_blanc.obj"));
        jetonBlanc->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        jetonBlanc->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
        jetonBlanc->setMaterialFlag(irr::video::EMF_COLOR_MATERIAL, true);
        jetonBlanc->setVisible(false);
    jetonNoir = sceneManager->addMeshSceneNode (sceneManager->getMesh ("Model/jeton_noir.obj"));
        jetonNoir->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        jetonNoir->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
        jetonNoir->setMaterialFlag(irr::video::EMF_COLOR_MATERIAL, true);
        jetonNoir->setVisible(false);

    chargerMesh(nWorld, sceneManager);
}

Tresor::~Tresor()
{
    // Release du tresor
    std::vector<Jeton*>::iterator it;
    for(it = tresor.begin(); it != tresor.end(); it++)
        delete *it;
}

void Tresor::chargerMesh(NewtonWorld* nWorld, irr::scene::ISceneManager* sceneManager)
{
    std::ifstream fichier("Model/pieces_positions.txt");
    char commande = 0;
    std::string type;
    TokenColor ctype = LINGOT;
    irr::scene::IMeshSceneNode* meshBase = NULL;
    float xt, yt, zt;
    float xr, yr, zr;

    while(commande != 'e')
    {
        fichier >> commande;
        switch(commande)
        {
            case 'o' :
            {
                fichier >> type;
                if(type == "lingot")
                {
                    ctype = LINGOT;
                    meshBase = lingot;
                }
                else if(type == "jeton_blanc")
                {
                    ctype = BLANC;
                    meshBase = jetonBlanc;
                }
                else if(type == "jeton_noir")
                {
                    ctype = NOIR;
                    meshBase = jetonNoir;
                }
                break;
            }
            case 'n' :
            {
                fichier >> xt >> yt >> zt >> xr >> yr >> zr;
                Jeton* jeton = new Jeton(nWorld, sceneManager, meshBase, ctype, irr::core::vector3df(xt,yt,zt), irr::core::vector3df(xr,yr,zr));
                tresor.push_back(jeton);
                break;
            }
            case '-':
                fichier.ignore(500,'\n');
            break;

            case '\n' : break;

            default :
                commande = 'e';
        };
    }

    fichier.close();
}

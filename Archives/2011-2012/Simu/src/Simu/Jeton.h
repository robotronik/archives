#ifndef JETON_H_INCLUDED
#define JETON_H_INCLUDED

#include <vector>
#include <fstream>
#include "IObject.h"

namespace Simu
{

enum TokenColor
{
    BLANC, NOIR, LINGOT
};


// Jeton de base, le blanc et noir seront enfants
class Jeton : public IObject
{
    public :
        Jeton(NewtonWorld* nWorld, irr::scene::ISceneManager* sceneManager, irr::scene::IMeshSceneNode* mesh, TokenColor couleur, irr::core::vector3df _position, irr::core::vector3df _rotation);
        int getPoints();
        TokenColor getType() const {return ctype;}

        static float getMasse(TokenColor ctype);

    private :
        // Points
        TokenColor ctype;

        // Newton
        NewtonCollision* creerEnveloppeComplexe(NewtonWorld* nWorld, irr::scene::ISceneManager* sceneManager, TokenColor couleur, irr::core::vector3df _position, irr::core::vector3df _rotation);
};


// L'ensemble des jetons sur la table
class Tresor
{
    public :
        Tresor(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld);
        ~Tresor();

        std::vector<Jeton*>::iterator getFirst()  {return tresor.begin();}
        bool isEnd(std::vector<Jeton*>::iterator& it) {return it == tresor.end();}

    private :
        std::vector<Jeton*> tresor;

        // Meshes
        irr::scene::IMeshSceneNode* jetonBlanc;
        irr::scene::IMeshSceneNode* jetonNoir;
        irr::scene::IMeshSceneNode* lingot;
        void chargerMesh(NewtonWorld* nWorld, irr::scene::ISceneManager* sceneManager);
};

}
#endif // JETON_H_INCLUDED

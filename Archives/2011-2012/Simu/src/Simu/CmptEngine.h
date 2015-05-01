#ifndef CMPTENGINE_H
#define CMPTENGINE_H

#include "IEngine.h"

#define ROUGE 0
#define VIOLET 1

#define SECONDES_PREMATCH 3
#define DUREE_MATCH_MINUTES 1
#define DUREE_MATCH_SECONDES 30

/*
    Toutes les methodes sont O(1)
*/
namespace Simu
{

class CmptEngine : public IEngine
{
    public:
        CmptEngine();
        ~CmptEngine();

        // Accesseurs
        // Utiles au GameEngine
        bool getDebutMatch() const {return debutMatch;}
        bool getFinMatch() const {return finMatch;}

        // Methodes
        void reinit();
        void run(float dt);

    private:
        unsigned int minutes;
        unsigned int secondes;
        float lastRetirerSeconde;
        int pointsViolet;
        int pointsRouge;
        bool debutMatch;
        bool finMatch;

        bool changed;

        bool retirerSeconde();
        void askPoints(); // Appelle le moteur physique
        void sendChangesToGraphicEngine(); // Appelle le moteur graphique

        void traiterMessages();
};

}
#endif // CMPTENGINE_H

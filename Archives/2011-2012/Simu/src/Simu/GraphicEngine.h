#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H

#include "config.h"

#include "IEngine.h"
#include <irr/irrlicht.h>
#include <ctime>

#define W_HEIGHT 810
#define W_WIDTH  1200
#define VIEW_HEIGHT W_HEIGHT/3

#define OFFSET_RIGHT_MAP 225

#include "Logger.h"

#ifdef DEBUG_CARTO
#include "../Soc/remplirObj.h"
#endif

#ifdef DEBUG_CLUSTER
extern Cluster objectifEnCours;
#include "../Soc/cluster.h"
#endif

#ifdef DEBUG_ESPION
#include "../Soc/espion.h"
//extern void afficher_espion();
#endif

// Map
#define DIM_RECT_ROBOT 10
#define DIM_RECT_JETON 5


namespace Simu
{

class AffichCompteur
{
    public :
        AffichCompteur(irr::video::IVideoDriver* driver);

        void drawCompteur(irr::scene::ISceneManager* sceneManager);



    private :
        irr::video::ITexture* tabChiffre[10];
        irr::video::ITexture* tabChiffreDepart[4];

        //Compteur
        int hr[3];
        int rg[3];
        int vi[3];
        int depart;

};

class GraphicEngine : public IEngine
{
    public:
        GraphicEngine();
        ~GraphicEngine();

        void run(float dt);

        irr::IrrlichtDevice* getDevice() const {return device;}
        irr::scene::ISceneManager* getSceneManager() const {return sceneManager;}
        irr::video::IVideoDriver* getDriver() const {return driver;}

        void sleep(int ms);


    private :
        bool doubleScreen;
        irr::IrrlichtDevice* device;
        irr::video::IVideoDriver* driver;
        irr::scene::ISceneManager* sceneManager;
        irr::gui::IGUIEnvironment *GUI;

        // Camera
        irr::scene::ICameraSceneNode* camGlobalView;
        irr::scene::ICameraSceneNode* camRobotView;

        void drawCollision(const IObject& obj);
        void drawSceneSimple();
        void drawSceneDouble();
        void drawGlobalView();
        void drawMap();
        void drawLog();
        #ifdef DEBUG_CLUSTER
        void drawCurrentObjectif();
        void drawCluster();
        void drawOneCluster(Cluster* cluster, const irr::video::SColor& couleur);
        #endif
        #ifdef DEBUG_ESPION
        void drawEspion();
        #endif
        void drawEObjectif(EObjectif& objectif,const irr::video::SColor& couleur);
        #ifdef DEBUG_CARTO
        void drawOnePath(int xd, int yd, int xa, int ya, const irr::video::SColor& couleur);
        void drawDepl(int& xd, int& yd, Depl depl, const irr::video::SColor& couleur);
        void drawPath();
        #endif
        irr::video::SColor genRandomCouleur();
        irr::video::SColor getRandomCouleur(int i);
        void drawCross(irr::core::position2d<irr::s32> pos, int taille, const irr::video::SColor& couleur);

        // Donnees
        irr::video::ITexture* fondCarte;
        irr::video::SColor randCouleur[27];
        void genRandCouleur();
        float compteurSecondes;

        // Outils
        void setLength(irr::core::position2d<irr::s32>& u, int length);

        // Logger
        irr::gui::IGUIStaticText *logger;
        void updateLogger();

        void traiterMessage();
};

void DebugShowGeometryCollision (void* driver, int vertexCount, const dFloat* faceVertec, int id);

}

#endif // GRAPHICENGINE_H

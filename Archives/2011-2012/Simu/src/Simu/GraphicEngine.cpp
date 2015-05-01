#include "GraphicEngine.h"

using namespace Simu;

AffichCompteur::AffichCompteur(irr::video::IVideoDriver* driver)
{
    //irr::video::ITexture* tabChiffre[10];

    for(int i=0; i<2; i++)
    {
        rg[i] = -1;
        vi[i] = -1;
        hr[i+1] = 0;
    }
    hr[0] = 2;
    rg[2] = 0;
    vi[2] = 0;

    depart = 3;
}

void AffichCompteur::drawCompteur(irr::scene::ISceneManager* sceneManager)
{

}

irr::video::IVideoDriver *g_driver = NULL;

GraphicEngine::GraphicEngine() :
IEngine(FREQ_IRRL),
doubleScreen(false),
device(irr::createDevice( // creation device
        irr::video::EDT_OPENGL,                      // l'API est OpenGL
        irr::core::dimension2d<irr::u32>(W_WIDTH, W_HEIGHT),   // taille de la fenetre 800x600
        32, false, true, false, 0)),
driver(device->getVideoDriver()),                    // creation driver
sceneManager(device->getSceneManager ()),
GUI(device->getGUIEnvironment()),
fondCarte(driver->getTexture("img/fondCarte.png")),
compteurSecondes(0)
{


    sceneManager->setAmbientLight(irr::video::SColorf(0.0,0.0,0.0,0.0));
    sceneManager->addLightSceneNode (NULL, irr::core::vector3df(10,10,10), irr::video::SColorf(1.0f,1.0f,1.0f,0.0f), 40.0f);

    camGlobalView = sceneManager->addCameraSceneNode (0,             // ajout camera fixe
        irr::core::vector3df (0,25,-10),
        irr::core::vector3df (0,0,0));
    camGlobalView->setAspectRatio(W_WIDTH/2/(W_HEIGHT-VIEW_HEIGHT));

    camRobotView = sceneManager->addCameraSceneNode ();
    camRobotView->setAspectRatio(W_WIDTH/2/(W_HEIGHT-VIEW_HEIGHT));

    g_driver = driver;

    genRandCouleur();

    logger = GUI->addStaticText(L"LOGGER\n"
                                ,irr::core::rect<irr::s32>(W_WIDTH/2-OFFSET_RIGHT_MAP+20,2*VIEW_HEIGHT,W_WIDTH/2+OFFSET_RIGHT_MAP-10,W_HEIGHT));
    logger->setOverrideColor(irr::video::SColor(255, 0, 255, 0));
}

GraphicEngine::~GraphicEngine()
{
    device->drop();
}

void GraphicEngine::sleep(int ms)
{
    device->sleep(ms, false);
}

void GraphicEngine::run(float dt)
{
    compteurSecondes += dt;
    traiterMessage();
    driver->beginScene(                          // demarre le rendu
        true,                                    // clear back-buffer
        true,                                    // clear z-buffer
        irr::video::SColor(255,255,255,255));    // fond blanc

    // Dessin de la scene 3D
    drawGlobalView();
    //if(doubleScreen)
        drawSceneDouble();
    //else
    //    drawSceneSimple();

    // Dessin 2D
    // Log
    drawLog();

    #ifdef DEBUG_ESPION
    drawEspion();
    #endif

    #ifdef DEBUG_CLUSTER
    drawCurrentObjectif();
    if(compteurSecondes<1)
        drawCluster();
    #endif

    #ifdef DEBUG_CARTO
    drawPath();
    #endif

    // True map
    drawMap();


    // On update le logger
    updateLogger();
    GUI->drawAll();

    driver->endScene ();

    if(compteurSecondes > 2)
        compteurSecondes -= 2;
}

void GraphicEngine::drawCollision(const IObject& obj)
{
    NewtonCollision* collision = NewtonBodyGetCollision(obj.getBody());
    irr::core::matrix4 mat;
    NewtonBodyGetMatrix(obj.getBody(), mat.pointer());
	NewtonCollisionForEachPolygonDo (collision, mat.pointer(),  DebugShowGeometryCollision, driver);
}

void GraphicEngine::drawSceneSimple()
{
    driver->setViewPort(irr::core::rect<irr::s32>(W_WIDTH/2,0, W_WIDTH,  W_HEIGHT-VIEW_HEIGHT));

    // On place la camera
    irr::core::matrix4 mat = data->getRobotRouge().getMatrix();
    irr::core::vector3df dir(0.0f,-0.5f, 1.0f), t(0.0f, 5.0f, -7.0f), pos, cible;
    mat.rotateVect(dir);
    mat.rotateVect(t);
    pos = mat.getTranslation();
    pos += t;
    cible = pos + dir;

    camRobotView->setPosition(pos);
    camRobotView->setTarget(cible);
    sceneManager->setActiveCamera(camRobotView);

    sceneManager->drawAll ();
}

void GraphicEngine::drawSceneDouble()
{

    // On calcule les cameras
    irr::core::matrix4 matR = data->getRobotRouge().getMatrix();
    irr::core::matrix4 matV = data->getRobotViolet().getMatrix();
    irr::core::vector3df dirR(0.0f,-0.5f, 1.0f), tR(0.0f, 5.0f, -7.0f), posR, cibleR;
    irr::core::vector3df dirV(0.0f,-0.5f, 1.0f), tV(0.0f, 5.0f, -7.0f), posV, cibleV;
    matR.rotateVect(dirR);    matV.rotateVect(dirV);
    matR.rotateVect(tR);      matV.rotateVect(tV);
    posR = matR.getTranslation();    posV = matV.getTranslation();
    posR += tR;    posV += tV;
    cibleR = posR + dirR;    cibleV = posV + dirV;


    // Rouge
    driver->setViewPort(irr::core::rect<irr::s32>(W_WIDTH/2,0, W_WIDTH,  (W_HEIGHT-VIEW_HEIGHT)/2));
    camRobotView->setPosition(posR);
    camRobotView->setTarget(cibleR);
    sceneManager->setActiveCamera(camRobotView);
    sceneManager->drawAll ();

    // Violet
    driver->setViewPort(irr::core::rect<irr::s32>(W_WIDTH/2,(W_HEIGHT-VIEW_HEIGHT)/2, W_WIDTH,  W_HEIGHT-VIEW_HEIGHT));
    camRobotView->setPosition(posV);
    camRobotView->setTarget(cibleV);
    sceneManager->setActiveCamera(camRobotView);
    sceneManager->drawAll ();
}


void GraphicEngine::drawGlobalView()
{
    sceneManager->setActiveCamera(camGlobalView);
    driver->setViewPort(irr::core::rect<irr::s32>(0,0, W_WIDTH/2, W_HEIGHT-VIEW_HEIGHT));
    sceneManager->drawAll ();                    // calcule le rendu

    // On dessine le debug
    drawCollision(data->getRobotRouge());
    drawCollision(data->getRobotRouge().getPinceGauche());
    drawCollision(data->getRobotRouge().getPinceDroite());
    /*
    //drawCollision(data->getRobotRouge());
    //drawCollision(data->getRobotViolet());
    //drawCollision(data->getPlateau());
    //*/
}

static irr::core::position2d<irr::s32> toDimMapLeft(float x, float y, int dimRect)
{
    // Width  : de 0 a 3000
    // Height : de 0 a 2000
    int _x = (int)(((3000.0f-x)/3000.0f) * 375.0f);
    int _y = (int)((y/2000.0f) * 250.0f);
    return irr::core::position2d<irr::s32>(10 + _x - dimRect/2, W_HEIGHT-VIEW_HEIGHT+10 + _y - dimRect/2);
}

static irr::core::position2d<irr::s32> toDimMapRight(float x, float y, int dimRect)
{
    // Width  : de -15 a 15
    // Height : de -10 a 10
    int _x = (int)(((x+15.0f)/30.0f) * 375.0f);
    int _y = (int)(((-y+10.0f)/20.0f) * 250.0f);
    return irr::core::position2d<irr::s32>(W_WIDTH/2+OFFSET_RIGHT_MAP + _x - dimRect/2, W_HEIGHT-VIEW_HEIGHT+10 + _y - dimRect/2);
}

void GraphicEngine::drawMap()
{
    driver->setViewPort(irr::core::rect<irr::s32>(0, 0, W_WIDTH, W_HEIGHT));
    driver->enableMaterial2D();

    // Affichage du fond
    driver->draw2DRectangle (irr::video::SColor(255,0,0,0), irr::core::rect<irr::s32>(W_WIDTH/2,  W_HEIGHT-VIEW_HEIGHT,W_WIDTH, W_HEIGHT));
    driver->draw2DImage(fondCarte, irr::core::rect <irr::s32>(W_WIDTH/2+OFFSET_RIGHT_MAP, W_HEIGHT-VIEW_HEIGHT+10,W_WIDTH/2+OFFSET_RIGHT_MAP+375,W_HEIGHT-VIEW_HEIGHT+10+250),
                        irr::core::rect <irr::s32>(0,0,375, 250));

    //Affichage des robots
    // Rouge
    irr::core::vector3df posRobot = data->getRobotRouge().getPos();
    irr::core::position2d<irr::s32> pos = toDimMapRight(posRobot.X, posRobot.Z, DIM_RECT_ROBOT);
    driver->draw2DRectangle (irr::video::SColor(255,255,0,0), irr::core::rect<irr::s32>(pos.X,  pos.Y, pos.X+DIM_RECT_ROBOT, pos.Y+DIM_RECT_ROBOT));
    // Violet
    posRobot = data->getRobotViolet().getPos();
    pos = toDimMapRight(posRobot.X, posRobot.Z, DIM_RECT_ROBOT);
    driver->draw2DRectangle (irr::video::SColor(255,125, 0,125), irr::core::rect<irr::s32>(pos.X,  pos.Y, pos.X+DIM_RECT_ROBOT, pos.Y+DIM_RECT_ROBOT));


    // Affichage des jetons
    irr::video::SColor couleur(255,0,0,0);
    irr::core::vector3df posJeton;
    for(std::vector<Jeton*>::iterator it = data->getTresor().getFirst(); !data->getTresor().isEnd(it); it++)
    {
        switch((*it)->getType())
        {
            case LINGOT : couleur = irr::video::SColor(255, 230, 210, 50); break;
            case BLANC : couleur = irr::video::SColor(255, 250, 250, 250); break;
            case NOIR : couleur = irr::video::SColor(255, 50, 50, 50); break;
        }
        posJeton = (*it)->getPos();
        pos = toDimMapRight(posJeton.X, posJeton.Z, DIM_RECT_JETON);
        driver->draw2DRectangle (couleur, irr::core::rect<irr::s32>(pos.X,  pos.Y, pos.X+DIM_RECT_JETON, pos.Y+DIM_RECT_JETON));
    }

    driver->enableMaterial2D(false);
}

void GraphicEngine::drawLog()
{
    driver->setViewPort(irr::core::rect<irr::s32>(0, 0, W_WIDTH, W_HEIGHT));
    driver->enableMaterial2D();

    // Affichage du fond
    driver->draw2DRectangle (irr::video::SColor(255,0,0,0), irr::core::rect<irr::s32>(10,  W_HEIGHT-VIEW_HEIGHT,W_WIDTH/2, W_HEIGHT));
    driver->draw2DImage(fondCarte, irr::core::rect <irr::s32>(10, W_HEIGHT-VIEW_HEIGHT+10,10+375,W_HEIGHT-VIEW_HEIGHT+10+250),
                        irr::core::rect <irr::s32>(0,0,375, 250));

    //Affichage des robots
    // Affichage des objectifs
    /*
    irr::video::SColor couleur(255,0,0,0);
    if(data->eObjectifs != NULL)
    {
        for(int i=0; i<NB_OBJECTIFS; i++)
        {
            switch(data->eObjectifs[i].type)
            {
                case 1 : couleur = irr::video::SColor(255, 250, 250, 250); break;
                case 2 : couleur = irr::video::SColor(255, 230, 210, 50); break;
                case 3 : couleur = irr::video::SColor(255, 125, 0, 0); break;
                case 4 : couleur = irr::video::SColor(255, 50, 0, 50); break;
                case 5 : couleur = irr::video::SColor(255, 125, 0, 0); break;
                case 6 : couleur = irr::video::SColor(255, 50, 0, 50); break;
                default : break;
            }
            if(data->eObjectifs[i].grabbed == 2)
                couleur = irr::video::SColor(255,255,0,0);
            drawEObjectif(data->eObjectifs[i], couleur);
        }
    }
    */

    driver->enableMaterial2D(false);


    // Affichage d'infos log
    int x =translateToXCard(Simu::DataSimu::getInstance()->getRobotViolet().getPos().X);
    int y =translateToYCard(Simu::DataSimu::getInstance()->getRobotViolet().getPos().Z);
    wchar_t texte[50];
    swprintf(texte, L"Violet : X : %d   Y : %d", x,y);
    setLog(5, texte);

    x =translateToXCard(Simu::DataSimu::getInstance()->getRobotRouge().getPos().X);
    y =translateToYCard(Simu::DataSimu::getInstance()->getRobotRouge().getPos().Z);
    swprintf(texte, L"Rouge : X : %d   Y : %d", x,y);
    setLog(6, texte);
}

void GraphicEngine::drawCross(irr::core::position2d<irr::s32> pos, int taille, const irr::video::SColor& couleur)
{
    irr::core::position2d<irr::s32> one = pos;
    irr::core::position2d<irr::s32> two = pos;

    one.X -= taille/2;
    two.Y -= taille/2;

    one.Y += taille/2;
    two.X += taille/2;
    driver->draw2DLine(one, two, couleur);


    one.X += taille;
    two.X -= taille;
    driver->draw2DLine(one, two, couleur);
}

#ifdef DEBUG_CLUSTER
void GraphicEngine::drawCurrentObjectif()
{
    // On place le cluster courant quand c'est besoin
    if(signaux.objectifDispo)
    {
        drawCross(toDimMapLeft(objectifEnCours.x, objectifEnCours.y, 0), DIM_RECT_ROBOT, irr::video::SColor(255, 50, 50, 255));
    }
}

void GraphicEngine::drawCluster()
{
    wchar_t texte[50];
    swprintf(texte, L"Nb de clusters : %d", clusters.nbClusters);
    setLog(7, texte);

    // On recupere les clusters
    for(unsigned int i=0; i<clusters.nbClusters; i++)
        drawOneCluster(&(clusters.v[i]), getRandomCouleur(i));
}

void GraphicEngine::drawOneCluster(Cluster* cluster, const irr::video::SColor& couleur)
{
    unsigned char i = 0;
    U64 id = 0x8000000000000000;
    for(i=0; i<NB_OBJECTIFS; i++)
    {
        if(cluster->objectifs & id)
            drawEObjectif(eObjectifs[i], couleur);
        id = id >> 1;
    }
}

#endif

#ifdef DEBUG_ESPION
void GraphicEngine::drawEspion()
{
    /* Le Robot adverse */
    // On affiche le carre correspondant au robot adverse
    RobotAdverse eRobotAdverse = getERobotAdverse();
    irr::core::position2d<irr::s32> pos = toDimMapLeft(eRobotAdverse.X, eRobotAdverse.Y, DIM_RECT_ROBOT);
    irr::core::position2d<irr::s32> offset(DIM_RECT_ROBOT/2,DIM_RECT_ROBOT/2);
    // Un carre pour la position
    driver->draw2DRectangle (irr::video::SColor(255,255, 0, 0), irr::core::rect<irr::s32>(pos.X,  pos.Y, pos.X+DIM_RECT_ROBOT, pos.Y+DIM_RECT_ROBOT));
    // Une ligne pour la direction
    irr::core::position2d<irr::s32> dir = irr::core::position2d<irr::s32>(eRobotAdverse.XDir, eRobotAdverse.YDir);
    setLength(dir, DIM_RECT_ROBOT*3);
    driver->draw2DLine(pos+offset, pos+dir+offset, irr::video::SColor(255,255, 0, 0));
    // Un carre pour l'ancienne pos
    pos = toDimMapLeft(eRobotAdverse.XOri, eRobotAdverse.YOri, DIM_RECT_ROBOT);
    driver->draw2DRectangle (irr::video::SColor(150,255, 100, 100), irr::core::rect<irr::s32>(pos.X,  pos.Y, pos.X+DIM_RECT_ROBOT, pos.Y+DIM_RECT_ROBOT));
    // Une ligne pour l'ancienne direction
    dir = irr::core::position2d<irr::s32>(eRobotAdverse.XDirOri, eRobotAdverse.YDirOri);
    setLength(dir, DIM_RECT_ROBOT*4);
    driver->draw2DLine(pos+offset, pos+dir+offset, irr::video::SColor(200,255, 100, 100));

    /* Notre robot */
    pos = toDimMapLeft(xRobot, yRobot, DIM_RECT_ROBOT);
    // Un carre pour la position
    driver->draw2DRectangle (irr::video::SColor(255,125, 0,125), irr::core::rect<irr::s32>(pos.X,  pos.Y, pos.X+DIM_RECT_ROBOT, pos.Y+DIM_RECT_ROBOT));


    // On affiche l etat probable du robot adverse
    switch(eRobotAdverse.state)
    {
        case GO_OBJ : setLog(2,L"Etat Adverse : GO_OBJ"); break;
        case GO_BASE : setLog(2, L"Etat Adverse : GO_BASE"); break;
        case GO_OUR_BASE : setLog(2, L"Etat Adverse : GO_OUR_BASE"); break;
        case GO_US : setLog(2, L"Etat Adverse : GO_US"); break;
        case BLOCK_US : setLog(2, L"Etat Adverse : BLOCK_US"); break;
        case DEPOSE : setLog(2, L"Etat Adverse : DEPOSE"); break;
        case GRAB : setLog(2, L"Etat Adverse : GRAB"); break;
        case GARDE : setLog(2, L"Etat Adverse : GARDE"); break;
        case BLOCK : setLog(2, L"Etat Adverse : BLOCK"); break;
    }
}
#endif

#ifdef DEBUG_CARTO
void GraphicEngine::drawOnePath(int xd, int yd, int xa, int ya, const irr::video::SColor& couleur)
{
    irr::core::position2d<irr::s32> depart = toDimMapLeft(xd, yd, 0);
    irr::core::position2d<irr::s32> arrivee = toDimMapLeft(xa, ya, 0);

    depart.X--; arrivee.X--;
    depart.Y--; arrivee.Y--;
    for(int i = 0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            driver->draw2DLine(depart, arrivee, couleur);
            depart.Y++; arrivee.Y++;
        }
        depart.X++; arrivee.X++;
        depart.Y -= 3; arrivee.Y -= 3;
    }
}

void GraphicEngine::drawDepl(int& xd, int& yd, Depl depl, const irr::video::SColor& couleur)
{
    if(depl.typeDepl != DEPL_ALPHA)
    {
        drawOnePath(xd, yd, depl.x, depl.y, couleur);
        xd = depl.x;
        yd = depl.y;
    }
}

void GraphicEngine::drawPath()
{
    int xDebut = xRobot;
    int yDebut = yRobot;
    Depl buff;

    // File dans l'asser
    for(int i=0; i<DataSimu::getInstance()->deplInAsser.taille; i++)
    {
        buff = DataSimu::getInstance()->deplInAsser.file[(i+DataSimu::getInstance()->deplInAsser.debut)%NB_DEPL_MAX];
        drawDepl(xDebut, yDebut, buff, irr::video::SColor(255, 50,255, 50));
        if(i == 0)
        {
            drawCross(toDimMapLeft(buff.x, buff.y, 0), DIM_RECT_ROBOT, irr::video::SColor(255, 255, 0, 0));
            wchar_t afficheDest[60] = {0};
            swprintf(afficheDest, L"Destination en cours : %d , %d", 3000- buff.x, buff.y);
            setLog(9, afficheDest);
        }
    }


    // File GO
    for(int i=0; i<fileDeplGo.taille; i++)
    {
        buff = fileDeplGo.file[(i+fileDeplGo.debut)%NB_DEPL_MAX];
        drawDepl(xDebut, yDebut, buff, irr::video::SColor(255, 50, 50, 255));
    }

    // File Return
    for(int i=0; i<fileDeplReturn.taille; i++)
    {
        buff = fileDeplReturn.file[(i+fileDeplReturn.debut)%NB_DEPL_MAX];
        drawDepl(xDebut, yDebut, buff, irr::video::SColor(255, 60, 0, 0));
    }

    // Repos
    /// TODO
}
#endif

void GraphicEngine::drawEObjectif(EObjectif& objectif, const irr::video::SColor& couleur)
{
    irr::core::position2d<irr::s32> pos;
    if(objectif.type > 0 && objectif.type < 7)
    {
        pos = toDimMapLeft(objectif.X, objectif.Y, DIM_RECT_JETON);
        driver->draw2DRectangle (couleur, irr::core::rect<irr::s32>(pos.X,  pos.Y, pos.X+DIM_RECT_JETON, pos.Y+DIM_RECT_JETON));
    }
}

void GraphicEngine::genRandCouleur()
{
    for(int i=0; i<27; i++)
    {
        randCouleur[i] = irr::video::SColor(255,
                            (i%3)*200+50,
                            ((i/3)%3)*200+50,
                            (i/3)*200+50);

    }
}

irr::video::SColor GraphicEngine::genRandomCouleur()
{
    int red, green, blue;

    red = rand()*100;
    green = rand()*100;
    blue = rand()*100;

    return irr::video::SColor(255, red, green, blue);
}

irr::video::SColor GraphicEngine::getRandomCouleur(int i)
{
    return randCouleur[i%27];
}

void GraphicEngine::setLength(irr::core::position2d<irr::s32>& u, int length)
{
    int norme = u.getLength();
    if(norme != 0)
    {
        u *= length;
        u /= norme;
    }
}

void GraphicEngine::updateLogger()
{
    wchar_t text [26*71];
    int i, j=0;
    for(i=0; i<26; i++)
    {
        wchar_t* textLog = Logger::getTextLog(i);
        wchar_t c = textLog[0];
        int k=0;
        while(c!=0)
        {
            text[j] = c;
            j++;
            k++;
            c = textLog[k];
        }
        text[j] = '\n';
        j++;
    }
    text[j-1] = 0;
    logger->setText(text);
}

void GraphicEngine::traiterMessage()
{
    Message m;
    while(!messages.empty())
    {
        m = messages.back();
        messages.pop_back();
        switch(m.type)
        {
            case MSG_ONE_PLAYER :
            {
                doubleScreen = false;
                camRobotView->setAspectRatio((W_WIDTH/2)/(W_HEIGHT-VIEW_HEIGHT));
                break;
            }
            case MSG_TWO_PLAYER :
            {
                doubleScreen = true;
                camRobotView->setAspectRatio((W_WIDTH/2)/((W_HEIGHT-VIEW_HEIGHT)/2));
                break;
            }

            default : break;
        }
    }
}

// Montrer la géometrie des collisions
namespace Simu
{

void DebugShowGeometryCollision (void* driver, int vertexCount, const dFloat* faceVertec, int id)
{
	int i;
    irr::video::IVideoDriver* _driver = (irr::video::IVideoDriver*) driver;
	i = vertexCount - 1;
	irr::core::vector3df p0 (faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
	for (i = 0; i < vertexCount; i ++)
	{
		irr::core::vector3df p1 (faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
		_driver->draw3DLine(p0, p1);
 		p0 = p1;
	}
}

}


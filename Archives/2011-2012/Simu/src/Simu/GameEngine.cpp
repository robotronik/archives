#include "GameEngine.h"

using namespace Simu;

GameEngine* Simu::GameEngine::instance = NULL;
bool enPause = false;

GameEngine::GameEngine() :
IEngine(FREQ_GAME),
// On commence par creer les moteurs 1 par 1
graphicEngine(),
physicEngine(),
joysEngine(graphicEngine.getDevice()),
cmptEngine(),
ia(),
continuer(true)
{
    instance = this;
    // On regle les adresses
    adresse.gameEngine = this;
    adresse.cmptEngine = &cmptEngine;
    adresse.graphicEngine = &graphicEngine;
    adresse.joysEngine = &joysEngine;
    adresse.physicEngine = &physicEngine;

    // On genere les data
    data = new DataSimu(graphicEngine.getSceneManager(), physicEngine.getWorld(), graphicEngine.getDevice()->getTimer());

    // On connecte le joystick
    graphicEngine.getDevice()->setEventReceiver(&joysEngine);

    previousTime = (DataSimu::getInstance()->getTimeTimer())/1000.0f;
}

GameEngine::~GameEngine()
{

}

void GameEngine::exec()
{
    static bool changedPause = false;
    while(graphicEngine.getDevice()->run())
    {
        if(enPause != changedPause)
        {
            changedPause = enPause;
            if(enPause)
            {
                data->stopTimer();
            }
            else
            {
                data->startTimer();
            }
        }
        if(enPause)
        {
            joysEngine.run(0);
            graphicEngine.sleep(300);
            continue;
        }
            // On prend le mieux
        std::vector<Proc>::iterator meilleur = processus.end();
        float pireRetard = 0.0f;
        float time = (DataSimu::getInstance()->getTimeTimer())/1000.0f; // ms -> s
        for(std::vector<Proc>::iterator it= processus.begin(); it != processus.end(); ++it)
        {

            it->retard = (time - it->lastAppel) - it->periode;
            if(it->retard > pireRetard)
            {
                pireRetard = it->retard;
                meilleur = it;
            }
        }

        if(meilleur != processus.end())
        {
            meilleur->dt = time - meilleur->lastAppel;
            meilleur->lastAppel = time;
            meilleur->nbAppels++;

            // Execution du processus
            IEngine* meilleurEngine = (IEngine*)meilleur->engine;
            meilleurEngine->run(meilleur->dt);
        }
    }
}

void GameEngine::run(float dt)
{


}

Simu::DataSimu* IEngine::data = NULL;
std::vector<Proc> IEngine::processus = std::vector<Proc>();
Adresse IEngine::adresse = {NULL};


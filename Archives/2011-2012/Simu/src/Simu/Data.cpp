#include "Data.h"

using namespace Simu;

DataSimu* DataSimu::instance = NULL;

DataSimu::DataSimu(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld, irr::ITimer* _timer) :
//eObjectifs(NULL),
plateau(sceneManager, nWorld),
tresor(sceneManager, nWorld),
violet(sceneManager, nWorld, irr::core::vector3df( 12, 0.1, -7.5), irr::core::vector3df(0, 90,0)),
rouge (sceneManager, nWorld, irr::core::vector3df(-12, 0.1, -7.5), irr::core::vector3df(0,-90,0)),
logger(),
timer(_timer)
{
    //ctor
    instance = this;

    deplInAsser.debut = 0;
    deplInAsser.taille = 0;

    timer->setTime(0);
    timer->start();
}

DataSimu::~DataSimu()
{
    //dtor
}

float DataSimu::getTimeTimer()
{
    return timer->getTime();
}

int translateToXCard(float x)
{
    return (int)(3000-((15+x)*100));
}

int translateToYCard(float y)
{
    return (int)(((10-y)*100));
}

void signalAsserDone()
{
    Simu::DataSimu::getInstance()->deplInAsser.debut = (Simu::DataSimu::getInstance()->deplInAsser.debut + 1)%NB_DEPL_MAX;
    Simu::DataSimu::getInstance()->deplInAsser.taille--;
}

void sendDeplSimu(Depl d)
{
    Simu::DataSimu::getInstance()->deplInAsser.file[(Simu::DataSimu::getInstance()->deplInAsser.debut+Simu::DataSimu::getInstance()->deplInAsser.taille)%NB_DEPL_MAX] = d;
    Simu::DataSimu::getInstance()->deplInAsser.taille++;
}

void viderDeplSimu()
{
    Simu::DataSimu::getInstance()->deplInAsser.debut = 0;
    Simu::DataSimu::getInstance()->deplInAsser.taille = 0;
}

int getTime()
{
    if(Simu::DataSimu::getInstance() == NULL)
    {
        static int falseTime = 100;
        falseTime += 100;
        return falseTime;
    }
    return (int)(Simu::DataSimu::getInstance()->getTimeTimer()*100);
}

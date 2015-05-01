#ifndef DATA_H
#define DATA_H

#include "uart.h"
#include "Plateau.h"
#include "Jeton.h"
#include "Robot.h"
#include "Logger.h"
#include "../Soc/cst.h"
//#include "../Soc/espion.h"

/*
    Cette classe contient toutes les donnees exploitables, modifiees et lues par les engines

*/

/*
violet(new Bot(irr::core::vector3df( 12, 0.1, -7.5), irr::core::vector3df(0,-90,0))),
rouge (new Bot(irr::core::vector3df(-12, 0.1, -7.5), irr::core::vector3df(0,90,0)))
*/

namespace Simu
{
class DataSimu
{
    public:

        DataSimu(irr::scene::ISceneManager* sceneManager, NewtonWorld* nWorld, irr::ITimer* _timer);
        ~DataSimu();

        Robot& getRobotRouge() {return rouge;}
        Robot& getRobotViolet() {return violet;}
        Plateau& getPlateau() {return plateau;}
        Tresor& getTresor() {return tresor;}

        float getTimeTimer();
        void startTimer() {timer->start();}
        void stopTimer() {timer->stop();}
        // DataSimu IA
        //EObjectif* eObjectifs;

        static DataSimu* getInstance() {return instance;}

        FileDepl deplInAsser;
    private:

        Plateau plateau;
        Tresor tresor;
        Robot violet, rouge;
        static DataSimu* instance;
        Logger logger;

        irr::ITimer* timer;
};


}

int translateToXCard(float x);
int translateToYCard(float y);

void signalAsserDone();
void sendDeplSimu(Depl d);
void viderDeplSimu();

int getTime();

#endif // DATA_H

#ifndef JOYSENGINE_H_INCLUDED
#define JOYSENGINE_H_INCLUDED

#include "IEngine.h"
#include <irr/irrlicht.h>

#define VITESSE_MAX 5.0f // 0.5 m/s
#define OMEGA 120.0f // un tour complet en 3 secondes
#define MIN_AXIS 15000.0f*15000.0f*2.0f
#define ACCEL 1.7f // En 3 secondes, on atteint vitesse max
#define BRAKE 3.5f // En moins de 1.5 secondes, on s'est arrete
#define DECEL 1.0f // En 5 secondes, on s'est arrete
#define DELAY_STOP_AV 0.1f; // 1 secondes avant de passer a la marche arriere
#define DELAY_STOP_AR 0.1f; // 1 secondes avant de passer a la marche arriere

#define ST_ARRET   0
#define ST_AVANT   1
#define ST_STOP_AV 2
#define ST_STOP_AR 3
#define ST_ARRIERE 4

extern bool enPause;

namespace Simu
{

struct Commande
{
    float vitesse;
    irr::core::vector3df du;    // Normalise a 0 ou 1
    bool ouverture;
    bool fermeture;
    bool accel;
    bool frein;
    char state;
    float delay;
    float angle;
    bool mutex;
};

inline void recalibre(float& value, float min, float max)
{
    if(value < min)
        value = min;
    else if(value > max)
        value = max;
}

class JoysEngine : public IEngine, public irr::IEventReceiver
{
    public:
        JoysEngine(irr::IrrlichtDevice* device);
        ~JoysEngine();

        virtual bool OnEvent(const irr::SEvent &event);

        void run(float dt);

    private :
        irr::core::array<irr::SJoystickInfo> joystickInfo;

        void traiterJoystickEvent(int player, irr::SEvent::SJoystickEvent event); // Player vaut 0 ou 1
        void traiterJoystickEvent(Commande& com, irr::SEvent::SJoystickEvent event);
        bool traiterKeyboardEvent(irr::SEvent::SKeyInput event);

        Commande comRouge;
        Commande comViolet;
        inline void resetCom(Commande& com)
        {
            com.vitesse = 0.0f;
            com.du = irr::core::vector3df(0.0f, 0.0f, 0.0f);
            com.ouverture = false;
            com.fermeture = false;
            com.accel = false;
            com.frein = false;
            com.state = ST_ARRET;
            com.delay = 0.0f;
            com.angle= 0.0f;
            com.mutex = true; // tu peux ecrire
        }
        inline void setComState(Commande &com, int state)
        {
            com.state = state;
            //printf("%d : vitesse : %lf\n", com.state, com.vitesse);
        }

        void recalibreAngle(Commande& com);
        void traiterCom(Commande& com, float dt);
        void envoiCom(Commande& com, Robot& robot, float dt);


        bool twoPlayers;
        void changerNbrJoueurs();
};


}

#endif // JOYSENGINE_H_INCLUDED

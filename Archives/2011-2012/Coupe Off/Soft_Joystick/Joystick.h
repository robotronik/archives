#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "Vecteur.h"
#include "UART.h"
#include <SDL\SDL.h>
#include <string>

#define MIN_AXIS 15000.0f*15000.0f*2.0f
#define MIN_CONFIG_AXIS 15000
#define ACCEL 2.0f // En 1 secondes, on atteint vitesse max
#define BRAKE 5.5f // En moins de 1.5 secondes, on s'est arrete
#define DECEL 2.5f // En 5 secondes, on s'est arrete
#define DELAY_STOP_AV 0.08f; // 1 secondes avant de passer a la marche arriere
#define DELAY_STOP_AR 0.08f; // 1 secondes avant de passer a la marche arriere

#define ST_ARRET   0
#define ST_AVANT   1
#define ST_STOP_AV 2
#define ST_STOP_AR 3
#define ST_ARRIERE 4

#define abs(a) ((a>=0) ? a : -a)
#define signe(a) ((a>=0) ? 1 : -1)

struct Commande
{
    float vitesse;
    Vecteur du;
    bool ouverture;
    bool fermeture;
    bool accel;
    bool frein;
    char state;
    float delay;
    float angle;
    bool mutex;
    bool nitroG;
    bool nitroD;
};

struct ConfigJoystick
{
    int boutonAccel;
    int boutonFrein;
    int boutonNitroG;
    int boutonNitroD;
    int axeVertical;
    int signeAxeAvant;
    int axeHorizonal;
    int signeAxeDroit;
};

inline void recalibre(float& value, float min, float max)
{
    if(value < min)
        value = min;
    else if(value > max)
        value = max;
}

class Joystick
{
    public:
        Joystick();
        virtual ~Joystick();

        bool configJoystick(SDL_Surface* screen);
        void run(float dt); // en secondes

    private:
        XBee xbee;
        SDL_Joystick* joystick;
        ConfigJoystick config;
        Commande comRobot;
        SDL_Surface* messagesConfig[7];

        inline void resetCom()
        {
            comRobot.vitesse = 0.0f;
            comRobot.du = Vecteur();
            comRobot.ouverture = false;
            comRobot.fermeture = false;
            comRobot.accel = false;
            comRobot.frein = false;
            comRobot.state = ST_ARRET;
            comRobot.delay = 0.0f;
            comRobot.angle= 0.0f;
            comRobot.mutex = true; // tu peux ecrire
            comRobot.nitroG = false;
            comRobot.nitroD = false;
        }
        inline void setComState(int state)
        {
            comRobot.state = state;
        }

        void traiterEvenement();
        void recalibreAngle();
        void traiterCom(float dt);
        void envoiCom(float dt);

        void afficherDemandeCommande(SDL_Surface* screen, int n);
        void prendreBouton(int n, int num);
        void prendreAxe(int n, int num, int signe);
};

#endif // JOYSTICK_H

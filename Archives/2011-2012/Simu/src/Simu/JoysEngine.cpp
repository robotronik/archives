#include "JoysEngine.h"

using namespace Simu;

JoysEngine::JoysEngine(irr::IrrlichtDevice* device) :
IEngine(FREQ_JOYS), twoPlayers(false)
{
    device->activateJoysticks(joystickInfo);

    resetCom(comRouge);
    resetCom(comViolet);
}

JoysEngine::~JoysEngine()
{

}

bool JoysEngine::OnEvent(const irr::SEvent &event)
{
    if(event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        return traiterKeyboardEvent(event.KeyInput);
    }
    if(event.EventType == irr::EET_JOYSTICK_INPUT_EVENT)
    {
        // Un joystick est connecte et il a genere un event automatique
        traiterJoystickEvent(event.JoystickEvent.Joystick, event.JoystickEvent);

        return true;
    }
    return false;
}

void JoysEngine::run(float dt)
{
    comRouge.mutex = false;
    comViolet.mutex = false;

    // Traitement de laCommande
    traiterCom(comRouge, dt);
    traiterCom(comViolet, dt);

    // Envoi de la commande
    envoiCom(comRouge, data->getRobotRouge(), dt);
    envoiCom(comViolet, data->getRobotViolet(), dt);

    comRouge.mutex = true;
    comViolet.mutex = true;
}

bool JoysEngine::traiterKeyboardEvent(irr::SEvent::SKeyInput event)
{
    if(event.Key == irr::KEY_UP)
    {
        comRouge.accel = event.PressedDown;
        return true;
    }
    if(event.Key == irr::KEY_DOWN)
    {
        comRouge.frein = event.PressedDown;
        return true;
    }
    if(event.Key == irr::KEY_LEFT)
    {
        if(event.PressedDown)
        {
            // Gauche
            comRouge.du.Z = 0.0f;
            comRouge.du.X = 1.0f;
        }
        else
        {
            // Droit devant
            comRouge.du.Z = -1.0f;
            comRouge.du.X = 0.0f;
        }
        return true;
    }
    if(event.Key == irr::KEY_RIGHT)
    {
        if(event.PressedDown)
        {
            // Droite
            comRouge.du.Z = 0.0f;
            comRouge.du.X = -1.0f;
        }
        else
        {
            // Droit devant
            comRouge.du.Z = -1.0f;
            comRouge.du.X = 0.0f;
        }
        return true;
    }
    if(event.Key == irr::KEY_KEY_L)
    {
        comRouge.ouverture = event.PressedDown;
        return true;
    }
    if(event.Key == irr::KEY_KEY_M)
    {
        comRouge.fermeture = event.PressedDown;
        return true;
    }

    if(twoPlayers)
    {
        if(event.Key == irr::KEY_KEY_Z)
        {
            comViolet.accel = event.PressedDown;
            return true;
        }
        if(event.Key == irr::KEY_KEY_S)
        {
            comViolet.frein = event.PressedDown;
            return true;
        }
        if(event.Key == irr::KEY_KEY_Q)
        {
            if(event.PressedDown)
            {
                // Gauche
                comViolet.du.Z = 0.0f;
                comViolet.du.X = 1.0f;
            }
            else
            {
                // Droit devant
                comViolet.du.Z = -1.0f;
                comViolet.du.X = 0.0f;
            }
            return true;
        }
        if(event.Key == irr::KEY_KEY_D)
        {
            if(event.PressedDown)
            {
                // Droite
                comViolet.du.Z = 0.0f;
                comViolet.du.X = -1.0f;
            }
            else
            {
                // Droit devant
                comViolet.du.Z = -1.0f;
                comViolet.du.X = 0.0f;
            }
            return true;
        }
        if(event.Key == irr::KEY_KEY_F)
        {
            comViolet.ouverture = event.PressedDown;
            return true;
        }
        if(event.Key == irr::KEY_KEY_G)
        {
            comViolet.fermeture = event.PressedDown;
            return true;
        }
    }

    if(event.Key == irr::KEY_F1)
    {
        if(event.PressedDown)
        {
            changerNbrJoueurs();
        }
        return true;
    }
    if(event.Key == irr::KEY_KEY_P)
    {
        enPause = !enPause;
    }

    return false;
}

void JoysEngine::traiterJoystickEvent(int player, irr::SEvent::SJoystickEvent event)
{
    if(player == 0)
        traiterJoystickEvent(comRouge, event);
    else
        traiterJoystickEvent(comViolet, event);
}

void JoysEngine::traiterJoystickEvent(Commande & com, irr::SEvent::SJoystickEvent event)
{

    if(com.mutex)
    {
        // Ecriture de Commande
        /*
            g/d : 0 <0 / 0 > 0
            h/b : 1 <0 / 1 > 0
        */
        com.accel = event.IsButtonPressed(0);
        com.frein = (event.IsButtonPressed(1));
        com.ouverture = event.IsButtonPressed(2);
        com.fermeture = event.IsButtonPressed(3);

        // Axes
        com.du.Z = -abs((float)event.Axis[1]);
        com.du.X = -(float)event.Axis[0];

        // Normalisation
        if(com.du.getLengthSQ() > MIN_AXIS)
        {
            com.du.normalize();
        }
        else
        {
            // Droit devant
            com.du.Z = -1.0f;
            com.du.X = 0.0f;
        }
    }
}

void JoysEngine::traiterCom(Commande& com, float dt)
{
    switch(com.state)
    {
        case ST_ARRET :
        {
            if(com.accel)
            {
                com.vitesse = ACCEL*dt;
                setComState(com,ST_AVANT);
            }
            else if(com.frein)
            {
                com.vitesse = -ACCEL*dt;
                setComState(com,ST_ARRIERE);
            }
            break;
        }

        case ST_AVANT :
        {
            if(com.frein)
               com.vitesse -= BRAKE * dt;
            else if(com.accel)
                com.vitesse += ACCEL* dt;
            else
                com.vitesse -= DECEL * dt;

            recalibre(com.vitesse, 0.0f, VITESSE_MAX);
            if(com.vitesse == 0.0f)
            {
                com.delay = DELAY_STOP_AV;
                setComState(com,ST_STOP_AV);
            }

            break;
        }
        case ST_ARRIERE :
        {
            if(com.accel)
               com.vitesse += BRAKE * dt;
            else if(com.frein)
                com.vitesse -= ACCEL* dt;
            else
                com.vitesse += DECEL * dt;

            recalibre(com.vitesse, -VITESSE_MAX, 0.0f);
            if(com.vitesse == 0.0f)
            {
                com.delay = DELAY_STOP_AR;
                setComState(com,ST_STOP_AR);
            }

            break;
        }

        case ST_STOP_AV :
        {
            if(com.accel)
            {
                com.vitesse = ACCEL*dt;
                setComState(com,ST_AVANT);
            }
            else
            {
                com.delay -= dt;
                if(com.delay <= 0.0f)
                    setComState(com,ST_ARRET);
            }
            break;
        }

        case ST_STOP_AR :
        {
            if(com.frein)
            {
                com.vitesse = -ACCEL*dt;
                setComState(com,ST_ARRIERE);
            }
            else
            {
                com.delay -= dt;
                if(com.delay <= 0.0f)
                    setComState(com,ST_ARRET);
            }
            break;
        }
    }

    if(com.vitesse != 0)
        recalibreAngle(com);
}

void JoysEngine::recalibreAngle(Commande& com)
{
    com.angle = asin(-com.du.X) * 180.0f/M_PI;
    float alphaMax = 1.0+10.0*(VITESSE_MAX - abs(com.vitesse))/VITESSE_MAX;

    com.angle = com.angle * alphaMax / 90.0f;
    com.angle *= (com.vitesse >= 0.0f) ? 1.0f : -1.0;

    // On place le du avec l'angle correspondant
    com.du.Z = -cos(com.angle*M_PI/180.0f);
    com.du.X = sin(com.angle*M_PI/180.0f);
}

void JoysEngine::envoiCom(Commande& com, Robot& robot, float dt)
{
    if(com.vitesse != 0.0f)
    {
        // Calcul de la vitesse
        irr::core::vector3df vit = com.du;
        vit *= com.vitesse;

        // Envoi de la commande
        robot.move(vit, com.angle, dt, data->getPlateau());
    }
    else if(com.state == ST_ARRET)
    {
        // Si on est bien a gauche ou bien a droite on tourne sur place
        if(com.du.X > 0.5f)
        {
            // envoi de la commande gauche
            robot.move(irr::core::vector3df(0,0,0), -OMEGA*dt, dt, data->getPlateau());
        }
        else if(com.du.X < -0.5f)
        {
            // envoi de la commande droite
            robot.move(irr::core::vector3df(0,0,0), OMEGA*dt, dt, data->getPlateau());
        }
    }

    // Envoi des commandes d'ouverture
}

void JoysEngine::changerNbrJoueurs()
{
    twoPlayers = !twoPlayers;

    // On envoie la demande au graphicengine
    if(twoPlayers)
        sendMessage(adresse.graphicEngine, MSG_TWO_PLAYER, NULL, false);
    else
        sendMessage(adresse.graphicEngine, MSG_ONE_PLAYER, NULL, false);
}

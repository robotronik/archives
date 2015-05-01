#include "Joystick.h"

Joystick::Joystick() : xbee()
{
    //ctor
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
    resetCom();

    char fichierBmp[] = "ecran_X.bmp";
    for(int i=0; i<7; i++)
    {
        sprintf(fichierBmp, "ecran_%d.bmp", i);
        messagesConfig[i] = SDL_LoadBMP(fichierBmp);
    }
}

Joystick::~Joystick()
{
    //dtor
    SDL_JoystickClose(joystick);
    for(int i=0; i<7; i++)
        SDL_FreeSurface(messagesConfig[i]);
}

void Joystick::traiterEvenement()
{
    // Ecriture de Commande
    /*
        g/d : 0 <0 / 0 > 0
        h/b : 1 <0 / 1 > 0
    */
    SDL_JoystickUpdate();
    comRobot.accel = SDL_JoystickGetButton(joystick, config.boutonAccel);
    comRobot.frein = SDL_JoystickGetButton(joystick, config.boutonFrein);
    comRobot.nitroG = SDL_JoystickGetButton(joystick, config.boutonNitroG);
    comRobot.nitroD = SDL_JoystickGetButton(joystick, config.boutonNitroD);
    //comRobot.ouverture = SDL_JoystickGetButton(joystick, 2);
    //comRobot.fermeture = SDL_JoystickGetButton(joystick, 3);

/// TODO -?
    comRobot.du.X = config.signeAxeAvant*((float)SDL_JoystickGetAxis(joystick, config.axeVertical));
    comRobot.du.Y = config.signeAxeDroit*((float)SDL_JoystickGetAxis(joystick, config.axeHorizonal));

    if(comRobot.du.getLengthSQ() > MIN_AXIS)
    {
        comRobot.du.normalize();
    }
    else
    {
        // Droit devant
        comRobot.du.Y = -1.0f;
        comRobot.du.X = 0.0f;
    }
}

bool Joystick::configJoystick(SDL_Surface* screen)
{
    // On entre les boutons un par un
    SDL_Event event;
    int nbCommande = 0;
    int numCommande;

    while(nbCommande < 4)
    {
        afficherDemandeCommande(screen, nbCommande);

        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                return false;
            case SDL_JOYBUTTONUP:
                if(nbCommande < 4)
                {
                    numCommande = event.jbutton.button;
                    nbCommande++;
                    break;
                }
            default : continue;
        }
        prendreBouton(nbCommande, numCommande);
    }


// Prise des axes
    int* axisValue = new int[SDL_JoystickNumAxes(joystick)];
    bool* axePris = new bool[SDL_JoystickNumAxes(joystick)];
    for(int i=0; i<SDL_JoystickNumAxes(joystick); i++)
    {
        axisValue[i] = SDL_JoystickGetAxis(joystick, i);
        axePris[i] = 0;
    }

    while(nbCommande < 6)
    {
        afficherDemandeCommande(screen, nbCommande);
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    delete [] axisValue;
                    delete [] axePris;
                    return false;
                default : break;
            }
        }

        for(int i=0; i<SDL_JoystickNumAxes(joystick); i++)
        {
            numCommande = SDL_JoystickGetAxis(joystick, i);
            if(numCommande != axisValue[i] && !axePris[i] && abs(numCommande)>MIN_CONFIG_AXIS)
            {
                prendreAxe(nbCommande, i, signe(numCommande));
                nbCommande++;
                axePris[i] = 1;
                break;
            }
        }
    }

    afficherDemandeCommande(screen, nbCommande);

    delete [] axisValue;
    delete [] axePris;

    return true;
}

void Joystick::afficherDemandeCommande(SDL_Surface* screen, int n)
{
    SDL_BlitSurface(messagesConfig[n], NULL, screen, NULL);
    SDL_Flip(screen);
}

void Joystick::prendreBouton(int n, int num)
{
    switch(n)
    {
        case 0 : config.boutonAccel = num; break;
        case 1 : config.boutonFrein = num; break;
        case 2 : config.boutonNitroG = num; break;
        case 3 : config.boutonNitroD = num; break;
        default : break;
    }
}

void Joystick::prendreAxe(int n, int num, int signe)
{
    switch(n)
    {
        case 4 : config.axeVertical = num; config.signeAxeAvant = signe;  break;
        case 5 : config.boutonFrein = num; config.signeAxeDroit = signe;  break;
        default : break;
    }
}


void Joystick::run(float dt)
{
    comRobot.mutex = false;

    // Recalculs sur la map
    traiterEvenement();

    // Traitement de laCommande
    traiterCom(dt);

    // Envoi de la commande
    envoiCom(dt);

    comRobot.mutex = true;
}

void Joystick::recalibreAngle()
{
    comRobot.angle = asin(-comRobot.du.X) * 180.0f/M_PI;
    float alphaMax = 1.0+10.0*(VITESSE_MAX - abs(comRobot.vitesse))/VITESSE_MAX;

    comRobot.angle = comRobot.angle * alphaMax / 90.0f;
    comRobot.angle *= (comRobot.vitesse >= 0.0f) ? 1.0f : -1.0;

    // On place le du avec l'angle correspondant
    comRobot.du.Y = -cos(comRobot.angle*M_PI/180.0f);
    comRobot.du.X = sin(comRobot.angle*M_PI/180.0f);
}

void Joystick::traiterCom(float dt)
{
    switch(comRobot.state)
    {
        case ST_ARRET :
        {
            if(comRobot.accel)
            {
                comRobot.vitesse = ACCEL*dt;
                setComState(ST_AVANT);
            }
            else if(comRobot.frein)
            {
                comRobot.vitesse = -ACCEL*dt;
                setComState(ST_ARRIERE);
            }
            break;
        }

        case ST_AVANT :
        {
            if(comRobot.frein)
               comRobot.vitesse -= BRAKE * dt;
            else if(comRobot.accel)
                comRobot.vitesse += ACCEL* dt;
            else
                comRobot.vitesse -= DECEL * dt;

            recalibre(comRobot.vitesse, 0.0f, VITESSE_MAX);
            if(comRobot.vitesse == 0.0f)
            {
                comRobot.delay = DELAY_STOP_AV;
                setComState(ST_STOP_AV);
            }

            break;
        }
        case ST_ARRIERE :
        {
            if(comRobot.accel)
               comRobot.vitesse += BRAKE * dt;
            else if(comRobot.frein)
                comRobot.vitesse -= ACCEL* dt;
            else
                comRobot.vitesse += DECEL * dt;

            recalibre(comRobot.vitesse, -VITESSE_MAX, 0.0f);
            if(comRobot.vitesse == 0.0f)
            {
                comRobot.delay = DELAY_STOP_AR;
                setComState(ST_STOP_AR);
            }

            break;
        }

        case ST_STOP_AV :
        {
            if(comRobot.accel)
            {
                comRobot.vitesse = ACCEL*dt;
                setComState(ST_AVANT);
            }
            else
            {
                comRobot.delay -= dt;
                if(comRobot.delay <= 0.0f)
                    setComState(ST_ARRET);
            }
            break;
        }

        case ST_STOP_AR :
        {
            if(comRobot.frein)
            {
                comRobot.vitesse = -ACCEL*dt;
                setComState(ST_ARRIERE);
            }
            else
            {
                comRobot.delay -= dt;
                if(comRobot.delay <= 0.0f)
                    setComState(ST_ARRET);
            }
            break;
        }
    }

    if(comRobot.vitesse != 0)
        recalibreAngle();

}

void Joystick::envoiCom(float dt)
{
    if(comRobot.vitesse != 0.0f)
    {
        // Envoi de la commande
        xbee.sendVitAlpha(comRobot.vitesse, comRobot.angle, comRobot.nitroG, comRobot.nitroD);
    }
    else if(comRobot.state == ST_ARRET)
    {
        // Si on est bien a gauche ou bien a droite on tourne sur place
        if(comRobot.du.X > 0.5f)
        {
            // envoi de la commande gauche
            xbee.sendVitAlpha(0, -OMEGA*dt, comRobot.nitroG, comRobot.nitroD);
        }
        else if(comRobot.du.X < -0.5f)
        {
            // envoi de la commande droite
            xbee.sendVitAlpha(0, OMEGA*dt, comRobot.nitroG, comRobot.nitroD);
        }
        else
        {
            xbee.sendVitAlpha(0,0, false, false);
        }
    }
}

#include "Joystick.h"

/*=============================================================================
 *
 *
 *                      Control du robot par joystick
 *
 *
 *===========================================================================*/


static bool traitantEvenement();



int main ( int argc, char** argv )
{
    /* printf dans la console avec la sdl */
    freopen("CON", "w", stdout);
    freopen("CON", "r", stdin);
    freopen("CON", "w", stderr);

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);

    SDL_Surface* screen = SDL_SetVideoMode(400, 200,32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Need For Speed - Robotronik", NULL);

    Joystick* joys = new Joystick;

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_Flip(screen);

    bool programmeOn = true;
    programmeOn = joys->configJoystick(screen);

    SDL_EnableKeyRepeat(10, 10);

    unsigned int tempsActuel = 0;
    unsigned int tempsPrecedent = 0;
    while(programmeOn)
    {
        tempsActuel = SDL_GetTicks();
        programmeOn = traitantEvenement();
        if(tempsActuel - tempsPrecedent > PERIODE_PROG)
        {
            joys->run(((float)(tempsActuel - tempsPrecedent))/1000.0f);
            tempsPrecedent = tempsActuel;
        }
        else
        {
            SDL_Delay(PERIODE_PROG - (tempsActuel - tempsPrecedent));
        }
    }

    delete joys;

    SDL_free(screen);
    SDL_Quit();


    return EXIT_SUCCESS;
}


static bool traitantEvenement()
{
    static SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                return false;
            default : break;
        }
    }


    return true;
}


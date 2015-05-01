

/*=============================================================================
 *
 *
 *          Control du robot par stick et visualisation
 *               de la cmd et du retour des moteurs
 *
 *
 *===========================================================================*/



/*------------------------------- include -----------------------------------*/


#include "main.hpp"
#include "uart.hpp"
#include "affichage.hpp"



/*------------------------------- variables ---------------------------------*/



SDL_Surface *screen;
static SDL_Joystick *joystick = NULL;
Robot *R;
Robot *R_Real;


int Vg = 0;
int Vd = 0;
int leftReel = 0;
int rightReel = 0;

//static SDL_TimerID timerSimu; /* Variable pour stocker le numéro du Timer */
static SDL_TimerID timerOneStepReel;
static SDL_TimerID timercheckUartRx;


extern HANDLE hComm;
extern OVERLAPPED Oo;
extern SDL_TimerID timerTestAsserAlive;
extern SDL_TimerID timeOutBetweenCodeAndData;




// null driver, advance serial data loger

/*------------------------------- declaration fonctions ---------------------*/


/*--------------------- main ----------------------------*/

static void exit_soft(void);
static void Joystick(void);


/*-------------------- deplacement robot reel -----------*/

Uint32 oneStepReel(Uint32 intervalle, void*);
static void updateCmdRobot(void);
Uint32 checkUartRx(Uint32 intervalle, void*);









/*---------------------------------------------------------------------------*/





int main ( int argc, char** argv )
{
    /* printf dans la console avec la sdl */
    freopen("CON", "w", stdout);
    freopen("CON", "r", stdin);
    freopen("CON", "w", stderr);

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);

    #ifdef START_UP
    printf("Start    , time = %6d\n", SDL_GetTicks());
    #endif

    //TTF_Init();

   // police7 = TTF_OpenFont("CulDeSac.ttf", 7*RAPPORT_HAUTEUR);
    screen = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32, SDL_HWSURFACE | SDL_DOUBLEBUF );
    SDL_WM_SetCaption("Simu cmd robot", NULL);
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_Flip(screen);

    SDL_EnableKeyRepeat(10, 10);

    Coord position_ini_robot;

    position_ini_robot.x = 150*RAPPORT_LARGEUR;
    position_ini_robot.y = (210-16.808)*RAPPORT_HAUTEUR;

    R = configuration_initiale_robot(SDL_MapRGB(screen->format, 1, 1, 1),position_ini_robot);
    R_Real = configuration_initiale_robot(SDL_MapRGB(screen->format, 255, 1, 1),position_ini_robot);

    tourner_a_90(R);
    tourner_a_90(R_Real);

    afficher_robot(R);
    afficher_robot(R_Real);
    SDL_Flip(screen);

    Oo = initCom();
    hComm = link(CONF_UART_115200_BAUD);

    if(hComm != NULL)
    {
        timerOneStepReel = SDL_AddTimer(TMP_ONE_STEP_REEL, oneStepReel, 0);
        timercheckUartRx = SDL_AddTimer(TMP_CHECK_UART_RX, checkUartRx, 0);
    }
    else
    {
        #ifdef XBEE_ERROR
        printf("Error start XBee\n");
        #endif 
    }

    #ifdef START_UP
    printf("Init done, time = %6d\n", SDL_GetTicks());
    #endif

    unsigned int tempsActuel = 0;
    unsigned int tempsPrecedent = 0;
    while(1)
    {
        tempsActuel = SDL_GetTicks();
        Joystick();
        if(tempsActuel - tempsPrecedent > TMP_FLIP_SIMU)
        {
            oneStepSimu();
            tempsPrecedent = tempsActuel;
        }
        else
        {
            SDL_Delay(TMP_FLIP_SIMU - (tempsActuel - tempsPrecedent));
        }
    }
    exit_soft();
    return EXIT_SUCCESS;
}


static void Joystick(void)
{
    static int vitesse_deplacement = 0;
    static int vitesse_rotation = 0;

    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            exit_soft();
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                exit_soft();
            }
            break;
        case SDL_JOYBUTTONDOWN:
            break;
        case SDL_JOYAXISMOTION:

            /* value max 32600 */
            if (event.jaxis.axis == 0 )
            {
                vitesse_rotation = event.jaxis.value;
            }
            else if (event.jaxis.axis == 1 )
            {
                vitesse_deplacement = -event.jaxis.value;
            }
            else if (event.jaxis.axis == 4 )
            {
                vitesse_rotation = event.jaxis.value;
            }
            else if (event.jaxis.axis == 3 )
            {
                vitesse_deplacement = -event.jaxis.value;
            }

            // if(vitesse_deplacement >= 0)
            // {
                Vd = vitesse_deplacement - vitesse_rotation;
                Vg = vitesse_deplacement + vitesse_rotation;
            // }
            // else
            // {
            //     Vd = vitesse_deplacement + vitesse_rotation;
            //     Vg = vitesse_deplacement - vitesse_rotation;
            // }

            break;
        default:
            break;
    }
}



static void exit_soft(void)
{
    #ifdef START_UP
    printf("Exit     , time = %6d\n", SDL_GetTicks());
    #endif

    SDL_JoystickClose(joystick);
//    SDL_RemoveTimer(timerSimu);
    SDL_RemoveTimer(timerOneStepReel);
    SDL_RemoveTimer(timercheckUartRx);
    SDL_RemoveTimer(timerTestAsserAlive);
    SDL_RemoveTimer(timeOutBetweenCodeAndData);

    if(hComm != NULL)
    {
        CloseHandle(hComm);
    }

    SDL_free(R->robot);
    free(R);
    SDL_free(screen);
    SDL_Quit();

    #ifdef START_UP
    printf("Exit done, time = %6d\n", SDL_GetTicks());
    #endif

    // fclose(stdout);
    // fclose(stdin);
    // fclose(stderr);

    exit(EXIT_SUCCESS);
}



/*------------------------------ deplacement robot reel ------------------------*/




Uint32 oneStepReel(Uint32 intervalle, void*)
{
        updateCmdRobot();

    // if(hComm != NULL)
    //     updateCmdRobot();
    // else;
    //     exit_soft();

    return intervalle;
}


static void updateCmdRobot(void)
{
    static int lastVg = 0;
    static int lastVd = 0;

    unsigned int update = 0;

    if( lastVg != Vg )
    {
        lastVg = Vg;
        update = 1;
    }
    if( lastVd != Vd )
    {
        lastVd = Vd;
        update = 1;
    }

    if(update)
    {

        #ifdef UART_CMD
        printf("Cmd  : left = %6d, right = %6d\n", Vg, Vd);
        //printf("update Vg = %5x, Vd = %5x\n", Vg, Vd);
        #endif

        int bouh = ((Vg*MAX_REEL)/(MAX_JOYSTICK*2)) << 16 | (unsigned short)((Vd*MAX_REEL)/(MAX_JOYSTICK*2));

        /* envoi des nouvelles cmd moteur a l'assert */
        //write_assert(ADRESSE_ASSER,MOTOR,bouh);
        write_assert(ADRESSE_PSOC,0x35,0);
    }

        //write_assert(19,0);
}


Uint32 checkUartRx(Uint32 intervalle, void*)
{
    //printf("check uart at %9d\n", SDL_GetTicks());
    readUart(hComm, &Oo);
    return intervalle;
}

void process(unsigned int code, int data)
{
    if(code == MOTOR)
    {
        //printf("bouh, %9d\n", SDL_GetTicks());
        leftReel = (int)(data >> 16);
        rightReel = (int)data;
    }
}




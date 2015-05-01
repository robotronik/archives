

// fonctions pour le lancement du programme



# include "main.hpp"

// Variables globales :

// du fichier de config
int BORDURES_BLANCHES;
int LARGEUR_FENETRE;
int HAUTEUR_FENETRE;
float RAPPORT_LARGEUR;//     LARGEUR_FENETRE/TAILLE_TABLE_LARGEUR
float RAPPORT_HAUTEUR;//     HAUTEUR_FENETRE/TAILLE_TABLE_HAUTEUR
int EPAISSEUR_PIXEL;
int DELAY;
float VITESSE_TRANS_ROBOT_DEFAUT;
float VITESSE_ROT_ROBOT_DEFAUT;
float VITESSE_PINCE_DEFAUT;
int ANGLE_PINCE_DEFAUT;
// du menu
int CONFIG = -1;
int DEBUG;
int orientation = 1;
// robot et instruction
Robot *R1, *R2;
Inst instructionRobot1;
Inst instructionRobot2;
int done1 = 0; // nb d'instruction effectué par le robot 1
int done2 = 0;

int tab[3];



SDL_Color couleurBlanc = {255, 255, 255};
SDL_Color couleurNoir = {0, 0, 0};
TTF_Font* police5;
TTF_Font* police6;
TTF_Font* police7;
TTF_Font* police8;
TTF_Font* police10;
SDL_Surface *screen;
SDL_Surface *robotronik = NULL, *fond = NULL;
SDL_Surface* infoState;
SDL_Surface* screen_save;
SDL_Surface* infoStateBlanc;
SDL_Surface* screen_save_menu;




SDL_Joystick *joystick = NULL;

void creationObjectAffichage(){
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
    TTF_Init();
    police5 = TTF_OpenFont("CulDeSac.ttf", 5*RAPPORT_HAUTEUR);
    police6 = TTF_OpenFont("CulDeSac.ttf", 6*RAPPORT_HAUTEUR);
    police7 = TTF_OpenFont("CulDeSac.ttf", 7*RAPPORT_HAUTEUR);
    police8 = TTF_OpenFont("CulDeSac.ttf", 8*RAPPORT_HAUTEUR);
    police10 = TTF_OpenFont("CulDeSac.ttf", 10*RAPPORT_HAUTEUR);
    screen = SDL_SetVideoMode(LARGEUR_FENETRE+BORDURES_BLANCHES,HAUTEUR_FENETRE+BORDURES_BLANCHES,32, SDL_HWSURFACE | SDL_DOUBLEBUF );
    infoStateBlanc = SDL_CreateRGBSurface(SDL_HWSURFACE, 150*RAPPORT_LARGEUR, 20, 32, 0, 0, 0, 0);
    SDL_FillRect(infoStateBlanc, NULL, SDL_MapRGB(infoStateBlanc->format, 200, 200, 200));
    robotronik = IMG_Load("robotronik.png");
    fond = IMG_Load("fond.jpg");
    iniFichierDebug();

    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);


}

void fermetureObjectAffichage(){
    SDL_free(screen);
    SDL_FreeSurface(robotronik);
    SDL_FreeSurface(fond);
    SDL_FreeSurface(infoState);
    SDL_FreeSurface(screen_save);
    SDL_FreeSurface(infoStateBlanc);
    SDL_FreeSurface(screen_save_menu);
    TTF_CloseFont(police5);
    TTF_CloseFont(police6);
    TTF_CloseFont(police7);
    TTF_CloseFont(police8);
    TTF_CloseFont(police10);
    closeFichierDebug();

    SDL_JoystickClose(joystick);

    TTF_Quit();
    SDL_Quit();
}

void finAppli(){
    fermetureObjectAffichage();
    exit(EXIT_SUCCESS);
}

void run()
{
    menu();
    if(tab[0]==1)
    {
        // mode debug : menu debug pour config strange
        menu_debug();
        DEBUG = 1;
        return;
    }
    creation_table();
    CONFIG = tab[1];
    initialiser_pion();
    Coord position_ini_robot;

    if(tab[2] == ROUGE)
    {
        position_ini_robot.x = 20*RAPPORT_LARGEUR;
        position_ini_robot.y = 17.5*RAPPORT_HAUTEUR;
        R1 = configuration_initiale_robot(SDL_MapRGB(screen->format, 255, 255, 0),position_ini_robot);
        position_ini_robot.x = (20+260)*RAPPORT_LARGEUR;
        position_ini_robot.y = 17.5*RAPPORT_HAUTEUR;
        R2 = configuration_initiale_robot(SDL_MapRGB(screen->format, 200, 200, 200),position_ini_robot);
        orientation = 1;
        tourner_a_180(R2);
    }
    else if(tab[2] == BLEU)
    {
        position_ini_robot.x = (20+260)*RAPPORT_LARGEUR;
        position_ini_robot.y = 17.5*RAPPORT_HAUTEUR;
        R1 = configuration_initiale_robot(SDL_MapRGB(screen->format, 255, 255, 0),position_ini_robot);
        position_ini_robot.x = 20*RAPPORT_LARGEUR;
        position_ini_robot.y = 17.5*RAPPORT_HAUTEUR;
        R2 = configuration_initiale_robot(SDL_MapRGB(screen->format, 200, 200, 200),position_ini_robot);
        orientation = -1;
        tourner_a_180(R1);
    }

    deplacement();

    free (R1);
    free (R2);
}


void fichier_config_prog()
{
    int i;
    FILE* config=NULL;
    config=fopen("config.txt", "r");
    double tab[9] = {0};
    char tab_ligne[1050] = "";
    // pour recreer le fichier de config si problème sur ce dernier
    /*
    char* ligne[30]={   "------------------------------ Valeurs de configuration ajustables                 ----------\n",
                        "\n",
                        "\n",
                        "------------------------------ Valeurs concernant l'affichage                      ----------\n",
                        "\n",
                        "BORDURES_BLANCHES        40.0\n",
                        "LARGEUR_FENETRE          900.0\n",
                        "HAUTEUR_FENETRE          630.0\n",
                        "// nb : resolution finale de la fenetre de [ LARGEUR_FENETRE + BORDURES_BLANCHES ; HAUTEUR_FENETRE + BORDURES_BLANCHES ]\n",
                        "// nb : largeur et hauteur respectivement multiples de 300 et 210 necessaires pour obtenir un comportement stable\n",
                        "\n",
                        "EPAISSEUR_PIXEL 2.0\n",
                        "\n",
                        "\n",
                        "\n",
                        "------------------------------ Valeur du temps de pause entre les generations (ms) ----------\n",
                        "\n",
                        "DELAY 1.0\n",
                        "// une valeur plus importante soulage l'ordinateur mais provoque une execution plus lente\n",
                        "\n",
                        "\n",
                        "\n",
                        "------------------------------ Valeurs par defaut concernant le robot 		   ----------\n",
                        "\n",
                        "VITESSE_TRANS_ROBOT_DEFAUT  0.3\n",
                        "// nb : ne pas la valeur de 0.3 (pas de un pixel) sinon les calcul et l'affichage ne se font plus de façon exact\n",
                        "VITESSE_ROT_ROBOT_DEFAUT    1.0\n",
                        "// nb : ne pas la valeur de 1 (pas de un degre) pour les même raisons\n",
                        "VITESSE_PINCE_DEFAUT        1.0\n",
                        "ANGLE_PINCE_DEFAUT          135.0\n"
                    };*/
    //*
    char* ligne[30]={   "------------------------------ Valeurs de configuration ajustables                 ----------\n",
                        "\n",
                        "\n",
                        "------------------------------ Valeurs concernant l'affichage                      ----------\n",
                        "\n",
                        "BORDURES_BLANCHES        %lf\n",
                        "LARGEUR_FENETRE          %lf\n",
                        "HAUTEUR_FENETRE          %lf\n",
                        "// nb : resolution finale de la fenetre de [ LARGEUR_FENETRE + BORDURES_BLANCHES ; HAUTEUR_FENETRE + BORDURES_BLANCHES ]\n",
                        "// nb : largeur et hauteur respectivement multiples de 300 et 210 necessaires pour obtenir un comportement stable\n",
                        "\n",
                        "EPAISSEUR_PIXEL %lf\n",
                        "\n",
                        "\n",
                        "\n",
                        "------------------------------ Valeur du temps de pause entre les generations (ms) ----------\n",
                        "\n",
                        "DELAY %lf\n",
                        "// une valeur plus importante soulage l'ordinateur mais provoque une execution plus lente\n",
                        "\n",
                        "\n",
                        "\n",
                        "------------------------------ Valeurs par defaut concernant le robot 		   ----------\n",
                        "\n",
                        "VITESSE_TRANS_ROBOT_DEFAUT  %lf\n",
                        "// nb : ne pas la valeur de 0.3 (pas de un pixel) sinon les calcul et l'affichage ne se font plus de façon exact\n",
                        "VITESSE_ROT_ROBOT_DEFAUT    %lf\n",
                        "// nb : ne pas la valeur de 1 (pas de un degre) pour les même raisons\n",
                        "VITESSE_PINCE_DEFAUT        %lf\n",
                        "ANGLE_PINCE_DEFAUT          %lf\n"
                    };//*/




    for(i=0;i<30;i++)
        strcat(tab_ligne, ligne[i]);

    fscanf(config, tab_ligne, &tab[0], &tab[1], &tab[2], &tab[3], &tab[4], &tab[5], &tab[6], &tab[7], &tab[8]);
    //fprintf(config, coucou);
    /*log("Valeur de configuration");
    for(i=0;i<9;i++)
        log_arg(tab[i]);*/
    BORDURES_BLANCHES = tab[0];
    LARGEUR_FENETRE = tab[1];
    HAUTEUR_FENETRE = tab[2];
    RAPPORT_LARGEUR = (float)LARGEUR_FENETRE/(float)TAILLE_TABLE_LARGEUR;
    RAPPORT_HAUTEUR = (float)HAUTEUR_FENETRE/(float)TAILLE_TABLE_HAUTEUR;
    EPAISSEUR_PIXEL = tab[3];
    DELAY = tab[4];
    VITESSE_TRANS_ROBOT_DEFAUT = tab[5];
    VITESSE_ROT_ROBOT_DEFAUT = tab[6];
    VITESSE_PINCE_DEFAUT = tab[7];
    ANGLE_PINCE_DEFAUT = tab[8];

    fclose(config);
}

int main ( int argc, char** argv )
{

    fichier_config_prog();
    creationObjectAffichage();
    run();
    fermetureObjectAffichage();
    return EXIT_SUCCESS;
}



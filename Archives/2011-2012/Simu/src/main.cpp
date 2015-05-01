#include "Simu\GameEngine.h"


/*
    Dimensions Simu / Reel
                1 u -> 100 mm, 0.1 m   : L dans reel en metres -> L*10 dans simu en u

    Axes :

    X : rouge vers violet
    Z : carte vers bouteilles

*/

using namespace Simu;

int main(void)
{
    GameEngine game;

    printf("Fin de l'initialisation\n");
    //system("PAUSE");
    system("CLS");

    game.exec();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "carto.h"

int main()
{
    /*int xNousDep = 300;
    int yNousDep = 1400;
    int xNousArr = 1750;
    int yNousArr = 350;
	int xAdv = 1500;
    int yAdv = 150;*/

    my_malloc_init();

    int xNousDep = 300;
    int yNousDep = 1700;
    int xNousArr = 2550;
    int yNousArr = 1000;
	int xAdv = 1500;
    int yAdv = 150;

	int longPlusChemin = 1;

	int nb;
	int longueur;

	Point list = NULL;
	Coor chemin = NULL;
	list = loadPointList();

	printf("0xABCD4321\n");


					list = loadAdversaire(xAdv,yAdv,list);
					chemin = aStar(xNousDep,yNousDep,xNousArr,yNousArr,list);
					longueur = calculLongueur(chemin);

					sendCoor(chemin,calculLongueur(chemin), longPlusChemin);

					nb = nbCoor(chemin);





    return 0;
}




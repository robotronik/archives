#include "help_main.h"

int xNousDep;
int yNousDep;
int xNousArr;
int yNousArr;
int xAdv;
int yAdv;
int longPlusChemin;
int longueur;
#ifdef DEUX_ROBOT_ADV
int xAdv2;
int yAdv2;
#endif

Point list = NULL;
Coor chemin = NULL;

void init_main()
{
	init();
	list = loadPointList();
	initToDo();
	initToToDo();
	LATBbits.LATB5 =1; //éteint
}

void boucle()
{
    if ( toDo.complet == 0b01 )
    {
        xNousDep = toDo.xNousDep;
        yNousDep = toDo.yNousDep;
        xNousArr = toDo.xNousArr;
        yNousArr = toDo.yNousArr;
        xAdv = toDo.xAdv;
        yAdv = toDo.yAdv;
        longPlusChemin = toDo.chemin;

        #ifdef DEUX_ROBOT_ADV
        loadAdversaire(xAdv,yAdv,xAdv2,yAdv2,list);
		#else
		loadAdversaire(xAdv,yAdv,list);
		#endif
        chemin = aStar(xNousDep,yNousDep,xNousArr,yNousArr,list);
        longueur = calculLongueur(chemin);

        sendCoor(chemin,calculLongueur(chemin), longPlusChemin);

        initToDo();
        my_free(NULL);
        
        #ifndef SIMU
        LATBbits.LATB5 = 1;//debug LED eteinte
        #endif
    }
}

#include "remplirObj.h"

FileDepl fileDeplGo;
FileDepl fileDeplReturn;
Depl deplRepos;
Cluster objectifEnCours;

void initRemplirObj()
{
    fileDeplGo.debut = 0;
    fileDeplGo.taille = 0;
    fileDeplReturn.debut = 0;
    fileDeplReturn.taille = 0;
}

void remplirCluster()
{
    // Plusieurs choix :
    // Circuit totem, Cale adverse, losange, unique
    objectifEnCours = objectifCourant;
    // Si c'est proche des totems, circuit totem
    if(X_MIN_TOTEM <= objectifCourant.x && X_MAX_TOTEM >= objectifCourant.x &&
       Y_MIN_TOTEM <= objectifCourant.y && Y_MAX_TOTEM >= objectifCourant.y)
       {
           remplirClusterCircuitTotem();
           return;
       }
    // Si c'est proche de la cale, cale
    if(estProcheZoneAdverse(objectifCourant.x, objectifCourant.y))
    {
        remplirClusterCaleAdverse();
        return;
    }
    // Si c'est proche du losange, losange
    if(X_MIN_LOSANGE <= objectifCourant.x && X_MAX_LOSANGE >= objectifCourant.x &&
       Y_MIN_LOSANGE <= objectifCourant.y && Y_MAX_LOSANGE >= objectifCourant.y)
       {
           remplirClusterLosange();
           return;
       }
    remplirClusterUnique();
}

void remplirClusterCircuitTotem()
{
    #ifdef SIMU
    wchar_t txt[50] = {0};
    swprintf(txt, L"Remplissage Circuit Totem");
    setLog(8, txt);
    #endif

    // On regarde le point d'acces sur le circuit et la direction et en voiture Simone
    EntreeTotem e = totemGetEntree();
    Depl predefinedPath[10];
    unsigned char last = (e.U) ? 4 : 2;
    unsigned int xDebut, yDebut;

    if(e.U)
    {
        xDebut = 1300;
        yDebut = 1250;
        predefinedPath[0].typeDepl = DEPL_ALPHA;
        predefinedPath[0].alpha = 180;
        predefinedPath[0].precision = PRECISION_DEFAULT_ALPHA_PRECIS;

        predefinedPath[1].typeDepl = DEPL_RAPIDE;
        predefinedPath[1].x = 1300;
        predefinedPath[1].y = 1250;
        predefinedPath[1].precision = PRECISION_CIRCUIT;

        predefinedPath[2].typeDepl = DEPL_RAPIDE;
        predefinedPath[2].x = 800;
        predefinedPath[2].y = 1250;
        predefinedPath[2].precision = PRECISION_CIRCUIT;

        predefinedPath[3].typeDepl = DEPL_RAPIDE;
        predefinedPath[3].x = 800;
        predefinedPath[3].y = 850;
        predefinedPath[3].precision = PRECISION_CIRCUIT;

        predefinedPath[4].typeDepl = DEPL_RAPIDE;
        predefinedPath[4].x = 900;
        predefinedPath[4].y = 850;
        predefinedPath[4].precision = PRECISION_CIRCUIT;

    }
    else
    {
        xDebut = 850;
        yDebut = 1300;
        predefinedPath[0].typeDepl = DEPL_ALPHA;
        predefinedPath[0].alpha = 0;
        predefinedPath[0].precision = PRECISION_DEFAULT_ALPHA_PRECIS;

        predefinedPath[1].typeDepl = DEPL_RAPIDE;
        predefinedPath[1].x = 2150;
        predefinedPath[1].y = 1300;
        predefinedPath[1].precision = PRECISION_CIRCUIT;
    }

    //pushFileDepl(&fileDeplGo, predefinedPath[0]);
    // On symetrise
    unsigned char i;
    if(!e.left)
    {
        xDebut= SYMX(xDebut);
        for(i=0; i<last; i++)
        {
            if(predefinedPath[i].typeDepl == DEPL_ALPHA)
                predefinedPath[i].alpha = SYMX_ALPHA(predefinedPath[i].alpha);
            else
                predefinedPath[i].x = SYMX(predefinedPath[i].x);

        }
    }
    if(!e.haut)
    {
        yDebut= SYMY(yDebut);
        for(i=0; i<last; i++)
        {
            if(predefinedPath[i].typeDepl == DEPL_ALPHA)
                predefinedPath[i].alpha = SYMY_ALPHA(predefinedPath[i].alpha);
            else
                predefinedPath[i].y = SYMX(predefinedPath[i].y);
        }
    }

    // On ajoute l'aller
    remplirCheminAller(xDebut, yDebut);

    // On fait le chemin predefini
    for(i=0; i<last; i++)
    {
        pushFileDepl(&fileDeplGo, predefinedPath[i]);
    }

    // On ajoute le retour
    remplirCheminRetour(predefinedPath[last-1].x, predefinedPath[last-1].y);

}

void remplirClusterCaleAdverse()
{
    #ifdef SIMU
    wchar_t txt[50] = {0};
    swprintf(txt, L"Remplissage Cale Adverse");
    setLog(8, txt);
    #endif

    // On va a la cale, on doit etre ouvert, on fait notre courbe, on referme, on recule en coube, demi-tour, retour
}

void remplirClusterLosange()
{
    #ifdef SIMU
    wchar_t txt[50] = {0};
    swprintf(txt, L"Remplissage Losange");
    setLog(8, txt);
    #endif

    // On regarde de quel sens on arrive du losange
    // de chez nous : U, de chez l'adversaire, simple englobage
}

void remplirClusterUnique()
{
    #ifdef SIMU
    wchar_t txt[50] = {0};
    swprintf(txt, L"Remplissage Unique");
    setLog(8, txt);
    #endif

    // On arrive, ouvert, on englobe, si la direction est bonne, on retourne, sinon, demi-tour et retour
}

void remplirCheminAller(int xa, int ya)
{
    // On vide la file courante
    viderFile(&fileDeplGo);
	askChemin(xRobot, yRobot, xa, ya, getERobotAdverse().X, getERobotAdverse().Y);
    /// TODO AJOUTER TIME OUT
    while(!signaux.pathReceived)
    {
        #ifdef SIMU
        forcerCarto();
		#else
		CyDelay(1);
        #endif
    }

    // On y va
    addRecepToDepl(&fileDeplGo);
}
void remplirCheminRetour(int xd, int yd)
{
    // On vide la file courante
    viderFile(&fileDeplReturn);
    // On obtient le chemin
	askChemin(xd, yd, X_RETURN, Y_RETURN, getERobotAdverse().X, getERobotAdverse().Y);
    /// TODO AJOUTER TIME OUT
    while(!signaux.pathReceived)
    {
        #ifdef SIMU
        forcerCarto();
		#else
		CyDelay(1);
        #endif
    }
    // On y va
    addRecepToDepl(&fileDeplReturn);

    // On place notre bon alpha, on recule, on se retourne
    Depl courant;

    courant.typeDepl = DEPL_ALPHA;
    courant.alpha = 180;
    courant.precision = PRECISION_DEFAULT_ALPHA_PRECIS;
    pushFileDepl(&fileDeplReturn, courant);

    courant.typeDepl = DEPL_RAPIDE;
    courant.x = X_FIN_RETURN;
    courant.y = Y_FIN_RETURN;
    courant.precision = PRECISION_DEFAULT_PRECIS;
    pushFileDepl(&fileDeplReturn, courant);

    courant.typeDepl = DEPL_ALPHA;
    courant.alpha = 0;
    courant.precision = PRECISION_DEFAULT_ALPHA_PRECIS;
    pushFileDepl(&fileDeplReturn, courant);
}

EntreeTotem totemGetEntree()
{
    EntreeTotem e;
    // 2 Types : centre ou excentre
    if(objectifCourant.y > 900 && objectifCourant.y < 1100) // Centre
    {
        e.left = (objectifCourant.x <= 1500);
        e.haut = (yRobot > 1000);
        if(e.left)
        {
            e.U = (xRobot > 1100);
        }
        else
        {
            e.U = (xRobot < 1900);
        }
    }
    else // excentre
    {
        // 4 points possibles -> 4 zones possibles pour notre robot
        // On cible le centre du cluster
        int x_robot = (objectifCourant.x <= 1500) ? xRobot : SYMX(xRobot);
        int y_robot = (objectifCourant.y > 1000)  ? yRobot : SYMY(yRobot);
        // On regarde la zone
        //Zone A
        if(x_robot < 1100)
        {
            e.left = 1;
            e.haut = 1;
            e.U = 0;
        }
        // Zone B
        else if(x_robot <= 1900 && y_robot >= 1000)
        {
            e.left = 1;
            e.haut = 1;
            e.U = 1;
        }
        // Zone D
        else if(3*y_robot<=-2*x_robot+6000)
        {
            e.left = 1;
            e.haut = 0;
            e.U = 1;
        }
        // Zone C
        else
        {
            e.left = 0;
            e.haut = 1;
            e.U = 0;
        }

        // On desymetrise
        if((objectifCourant.x > 1500))
            e.left = !e.left;
        if((objectifCourant.y <= 1000))
            e.haut = !e.haut;
    }

    return e;
}

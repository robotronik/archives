#include "espion.h"

EObjectif eObjectifs[NB_OBJECTIFS];
RobotAdverse eRobotAdverse;
int xRobot;
int yRobot;

RobotAdverse getERobotAdverse()
{
    return eRobotAdverse;
}

void initEspion()
{
    genJeton();
    initPosRobot(); // Initialisation de la liste
    initRobotAdverse();
    xRobot = X_DEBUT;
    yRobot = Y_DEBUT;
}



void espion()
{
	// Important pour bloquer le robot
	updatePos();

    // On traite une par une les arrivees de la tourelle
    /// TIME OUT renvoi ?
    char nombreEntrees = getSizePosRobot(0);
    char i;
    for(i=0; i<nombreEntrees; i++)
    {
        repositionnerJetons();
        traiterNouvellePosAdverse(popPosRobot(0));
    }

	// Danger fonction bloquante
	/// TODO ADD TIME OUT
	// J ai laisse du temps de calcul pour l asser
	while(!signaux.posRecue)
	{
		#ifndef SIMU
		CyDelay(1);
		#else
		forcerAsser();
		#endif
	}

	T_Pos_TabChar t = getLastPos();
    xRobot = t.x;
    yRobot = t.y;
    //printf("Alpha : %d\n", t.angle); /// TODO REGLER L ANGLE

    traiterNouvellePosANous(xRobot, yRobot);
}


void traiterNouvellePosANous(unsigned int x, unsigned int y)
{
    // On regarde si cette position nous est critique
    if(NSQ((signed)(x - eRobotAdverse.X)/10, (signed)(y - eRobotAdverse.Y)/10) < PROXIMITE_ADVERSE_FOR_COMP)
    {
        // Dans ce cas on bloque
        signaux.blocked = 1;
    }
    else
    {
        // debloque nous
        // Seul moyen de faire repasser le robot a un etat normal
        signaux.blocked = 0;
    }

    //
}

void traiterNouvellePosAdverse(EntreeFilePos pos)
{
    // On update le robot adverse
    updateRobotAdverse(pos);
}

void initRobotAdverse()
{
    eRobotAdverse.X = 2750;
    eRobotAdverse.XOri = 2750;
    eRobotAdverse.Y = 1750;
    eRobotAdverse.YOri = 1750;
    eRobotAdverse.XDir = -1;
    eRobotAdverse.XDirOri = -1;
    eRobotAdverse.YDir = 0;
    eRobotAdverse.YDirOri = 0;
    eRobotAdverse.VMax2 = 0;
    eRobotAdverse.t = 0;
    eRobotAdverse.state = GO_OBJ;
    eRobotAdverse.data.data = 0;
    eRobotAdverse.data.charge = 0;
}

void updateRobotAdverse(EntreeFilePos pos)
{
    RobotAdverse suivant;
    suivant.XDir = pos.X - eRobotAdverse.X;
    suivant.YDir = pos.Y - eRobotAdverse.Y;
    suivant.X = pos.X;
    suivant.Y = pos.Y;
    long long int dt = pos.t - eRobotAdverse.t;
    if(dt != 0)
    {
        suivant.VMax2 = (1000000*(suivant.XDir*suivant.XDir + suivant.YDir*suivant.YDir))/(dt*dt);
        if(eRobotAdverse.VMax2 < suivant.VMax2)
            suivant.VMax2 = suivant.VMax2;
        else
            suivant.VMax2 = eRobotAdverse.VMax2;
    }
    suivant.t = pos.t;
    if(changementDirOrigine(suivant))
    {
        suivant.XOri = suivant.X;
        suivant.YOri = suivant.Y;
        suivant.XDirOri = suivant.XDir;
        suivant.YDirOri = suivant.YDir;
    }

    // On update le state
    updateStateRobotAdverse(&suivant);

    // On deplace les jetons

    // On finit par update les positions
    eRobotAdverse = suivant;
}

void updateStateRobotAdverse(RobotAdverse* suivant)
{
    //GO_OBJ, GO_BASE, GO_OUR_BASE, GO_US, GARDE
    // Est-il en mouvement ?
    long long int dt = suivant->t - eRobotAdverse.t;
    char transporte = doGrabAdverse(suivant->X, suivant->Y);
    if(transporte)
        suivant->data.charge = 1;
    if(dt == 0 || (10000000000*NSQ(suivant->XDir,suivant->YDir))/(dt*dt) > V_MIN)
    {
        // Tout depend ensuite de la direction
        suivant->state = GO_OBJ;
    }
    else
    {
        // Tout depend dans la position
        // Si il est proche d un objectif
        if(transporte)
            suivant->state = GRAB;
        // Si il est proche de sa base
        else if(estProcheZoneAdverse(suivant->X, suivant->Y))
        {
            //et possede un jeton
            if(suivant->data.charge)
            {
                suivant->state = DEPOSE;
                deposerJeton(2);
                suivant->data.charge = 0;
            }
            // et ne possede pas de jeton
            else
            {
                suivant->state = GARDE;
            }
        }
        // Si il est proche de nous
        else if(estProcheDeNous(suivant->X, suivant->Y))
            suivant->state = BLOCK_US;
        // Sinon, bandes de gros nazes
        else
            suivant->state = BLOCK;
    }
}

char doGrabAdverse(int x, int y)
{
    int i;
    char retour = 0;
    for(i=0;i<NB_JETONS;i++)
    {
        // On regarde si il est proche
        int x2 = eObjectifs[i].X-x;
        int y2 = eObjectifs[i].Y-y;
        if(eObjectifs[i].grabbed == 0 &&
           !estDansZoneAdverse(eObjectifs[i].X, eObjectifs[i].Y) &&            // On considere que le robot ne repositionne pas les jetons
           (NSQ((long long int)(eObjectifs[i].X-x),(long long int)(eObjectifs[i].Y-y)) < PORTEE_ADVERSE))
        {
            retour = 1;
            eObjectifs[i].grabbed = 2;
        }
    }
    return retour;
}

char estDansZone(int x, int y)
{
    return ((x <= 500 && y >= 1500) ||  // Capitainerie
        (y <= 1500 && y >= 300*x-105000)); // Cale
}

char estProcheZone(int x, int y)
{
    return ((x <= 200 && y >= 1200) ||  // Capitainerie
        (y <= 1800 && y >= 300*x-195000)); // Cale
}

char estDansZoneAdverse(int x, int y)
{
    return estDansZone(SYMX(x), y);
}

char estDansCapitainerieAdverse(int x, int y)
{
    return (SYMX(x) <= 500 && y >= 1500);
}

char estDansCaleInaccessibleAdverse(int x, int y)
{
    return (y <= 700 && y >= 300*SYMX(x)-105000);
}

char estProcheZoneAdverse(int x, int y)
{
    return estProcheZone(SYMX(x),y);
}

char estProcheDeNous(int x, int y)
{
    return (NSQ((long long int)(x-xRobot),(long long int)(y-yRobot))<PROXIMITE_ADVERSE);
}

void deposerJeton(int id)
{
    int i;
    for(i=0; i<NB_JETONS; i++)
    {
        if(eObjectifs[i].grabbed == id)
            eObjectifs[i].grabbed = 0;
    }
}

void repositionnerJetons()
{
    int i;
    for(i=0; i<NB_JETONS; i++)
    {
        if(eObjectifs[i].grabbed == 1)  // A NOUS
        {
            eObjectifs[i].X = xRobot;
            eObjectifs[i].Y = yRobot;
            eObjectifs[i].Z = 0;
        }
        else if(eObjectifs[i].grabbed == 2)  // A EUX
        {
            eObjectifs[i].X = eRobotAdverse.X;
            eObjectifs[i].Y = eRobotAdverse.Y;
            eObjectifs[i].Z = 0;
        }
    }
}

char changementDirOrigine(RobotAdverse suivant)
{
    long long int ps = eRobotAdverse.XDirOri*suivant.XDir+ eRobotAdverse.YDirOri*suivant.YDir;
    if(ps <= 0)
        return 1;
    long long int na = NSQ(eRobotAdverse.XDirOri,eRobotAdverse.YDirOri);
    long long int nb = NSQ(suivant.XDir, suivant.YDir);
    if((ps*ps*100)/(na*nb) > 90) // comparaison avec un cos carré. Le x100 evite la division flottante
        return 1;
    return 0;
}

int eObjectifToPoint(EObjectif* objectif)
{
    switch(objectif->type)
    {
        case 1 : return 1;
        case 2 : return 3;
        case 3 : case 4 : case 5: case 6: return 5;
        default : return 0;
    }
}

#ifdef SIMU
void afficher_espion()
{
    printf("t  = %ld\n", eRobotAdverse.t);
    printf("Position RA : %d,%d - Direction RA : %d,%d\n", eRobotAdverse.X, eRobotAdverse.Y, eRobotAdverse.XDir, eRobotAdverse.YDir);
    printf("Position Macro : %d,%d - Direction Macro : %d,%d\n", eRobotAdverse.XOri, eRobotAdverse.YOri, eRobotAdverse.XDirOri, eRobotAdverse.YDirOri);
    printf("Vmax2 : %ld - ", eRobotAdverse.VMax2);
    afficher_state(eRobotAdverse.state);
}

void afficher_state(TypeStateAdverse state)
{
    printf("State : ");
    switch(state)
    {
        case GO_OBJ : printf("GO_OBJ"); break;
        case GO_BASE : printf("GO_BASE"); break;
        case GO_OUR_BASE : printf("GO_OUR_BASE"); break;
        case GO_US : printf("GO_US"); break;
        case BLOCK_US : printf("BLOCK_US"); break;
        case DEPOSE : printf("DEPOSE"); break;
        case GRAB : printf("GRAB"); break;
        case GARDE : printf("GARDE"); break;
        case BLOCK : printf("BLOCK"); break;
    }
    printf("\n");
}
#endif

void genJeton()
{
// 0
eObjectifs[0].type = 2;
eObjectifs[0].realise = 0;
eObjectifs[0].X = 1500;
eObjectifs[0].Y = 653;
eObjectifs[0].Z = 0;
eObjectifs[0].grabbed = 0;

// 1
eObjectifs[1].type = 2;
eObjectifs[1].realise = 0;
eObjectifs[1].X = 2564;
eObjectifs[1].Y = 1215;
eObjectifs[1].Z = 0;
eObjectifs[1].grabbed = 0;

// 2
eObjectifs[2].type = 2;
eObjectifs[2].realise = 0;
eObjectifs[2].X = 436;
eObjectifs[2].Y = 1215;
eObjectifs[2].Z = 0;
eObjectifs[2].grabbed = 0;

// 3
eObjectifs[3].type = 2;
eObjectifs[3].realise = 0;
eObjectifs[3].X = 1900;
eObjectifs[3].Y = 911;
eObjectifs[3].Z = 2;
eObjectifs[3].grabbed = 0;

// 4
eObjectifs[4].type = 2;
eObjectifs[4].realise = 0;
eObjectifs[4].X = 1900;
eObjectifs[4].Y = 1089;
eObjectifs[4].Z = 2;
eObjectifs[4].grabbed = 0;

// 5
eObjectifs[5].type = 2;
eObjectifs[5].realise = 0;
eObjectifs[5].X = 1100;
eObjectifs[5].Y = 911;
eObjectifs[5].Z = 2;
eObjectifs[5].grabbed = 0;

// 6
eObjectifs[6].type = 2;
eObjectifs[6].realise = 0;
eObjectifs[6].X = 1100;
eObjectifs[6].Y = 1089;
eObjectifs[6].Z = 2;
eObjectifs[6].grabbed = 0;

// 7
eObjectifs[7].type = 1;
eObjectifs[7].realise = 0;
eObjectifs[7].X = 1500;
eObjectifs[7].Y = 391;
eObjectifs[7].Z = 0;
eObjectifs[7].grabbed = 0;

// 8
eObjectifs[8].type = 1;
eObjectifs[8].realise = 0;
eObjectifs[8].X = 1500;
eObjectifs[8].Y = 210;
eObjectifs[8].Z = 0;
eObjectifs[8].grabbed = 0;

// 9
eObjectifs[9].type = 1;
eObjectifs[9].realise = 0;
eObjectifs[9].X = 1000;
eObjectifs[9].Y = 1500;
eObjectifs[9].Z = 0;
eObjectifs[9].grabbed = 0;

// 10
eObjectifs[10].type = 1;
eObjectifs[10].realise = 0;
eObjectifs[10].X = 2000;
eObjectifs[10].Y = 1500;
eObjectifs[10].Z = 0;
eObjectifs[10].grabbed = 0;

// 11
eObjectifs[11].type = 1;
eObjectifs[11].realise = 0;
eObjectifs[11].X = 495;
eObjectifs[11].Y = 300;
eObjectifs[11].Z = 0;
eObjectifs[11].grabbed = 0;

// 12
eObjectifs[12].type = 1;
eObjectifs[12].realise = 0;
eObjectifs[12].X = 2505;
eObjectifs[12].Y = 300;
eObjectifs[12].Z = 0;
eObjectifs[12].grabbed = 0;

// 13
eObjectifs[13].type = 1;
eObjectifs[13].realise = 0;
eObjectifs[13].X = 1100;
eObjectifs[13].Y = 1250;
eObjectifs[13].Z = 0;
eObjectifs[13].grabbed = 0;

// 14
eObjectifs[14].type = 1;
eObjectifs[14].realise = 0;
eObjectifs[14].X = 1900;
eObjectifs[14].Y = 1250;
eObjectifs[14].Z = 0;
eObjectifs[14].grabbed = 0;

// 15
eObjectifs[15].type = 1;
eObjectifs[15].realise = 0;
eObjectifs[15].X = 850;
eObjectifs[15].Y = 1000;
eObjectifs[15].Z = 0;
eObjectifs[15].grabbed = 0;

// 16
eObjectifs[16].type = 1;
eObjectifs[16].realise = 0;
eObjectifs[16].X = 2150;
eObjectifs[16].Y = 1000;
eObjectifs[16].Z = 0;
eObjectifs[16].grabbed = 0;

// 17
eObjectifs[17].type = 1;
eObjectifs[17].realise = 0;
eObjectifs[17].X = 925;
eObjectifs[17].Y = 825;
eObjectifs[17].Z = 0;
eObjectifs[17].grabbed = 0;

// 18
eObjectifs[18].type = 1;
eObjectifs[18].realise = 0;
eObjectifs[18].X = 2075;
eObjectifs[18].Y = 825;
eObjectifs[18].Z = 0;
eObjectifs[18].grabbed = 0;

// 19
eObjectifs[19].type = 1;
eObjectifs[19].realise = 0;
eObjectifs[19].X = 925;
eObjectifs[19].Y = 1175;
eObjectifs[19].Z = 0;
eObjectifs[19].grabbed = 0;

// 20
eObjectifs[20].type = 1;
eObjectifs[20].realise = 0;
eObjectifs[20].X = 2075;
eObjectifs[20].Y = 1175;
eObjectifs[20].Z = 0;
eObjectifs[20].grabbed = 0;

// 21
eObjectifs[21].type = 1;
eObjectifs[21].realise = 0;
eObjectifs[21].X = 1275;
eObjectifs[21].Y = 825;
eObjectifs[21].Z = 0;
eObjectifs[21].grabbed = 0;

// 22
eObjectifs[22].type = 1;
eObjectifs[22].realise = 0;
eObjectifs[22].X = 1725;
eObjectifs[22].Y = 825;
eObjectifs[22].Z = 0;
eObjectifs[22].grabbed = 0;

// 23
eObjectifs[23].type = 1;
eObjectifs[23].realise = 0;
eObjectifs[23].X = 1275;
eObjectifs[23].Y = 1175;
eObjectifs[23].Z = 0;
eObjectifs[23].grabbed = 0;

// 24
eObjectifs[24].type = 1;
eObjectifs[24].realise = 0;
eObjectifs[24].X = 1725;
eObjectifs[24].Y = 1175;
eObjectifs[24].Z = 0;
eObjectifs[24].grabbed = 0;

// 25
eObjectifs[25].type = 1;
eObjectifs[25].realise = 0;
eObjectifs[25].X = 1025;
eObjectifs[25].Y = 925;
eObjectifs[25].Z = 3;
eObjectifs[25].grabbed = 0;

// 26
eObjectifs[26].type = 1;
eObjectifs[26].realise = 0;
eObjectifs[26].X = 1975;
eObjectifs[26].Y = 925;
eObjectifs[26].Z = 3;
eObjectifs[26].grabbed = 0;

// 27
eObjectifs[27].type = 1;
eObjectifs[27].realise = 0;
eObjectifs[27].X = 1025;
eObjectifs[27].Y = 1075;
eObjectifs[27].Z = 3;
eObjectifs[27].grabbed = 0;

// 28
eObjectifs[28].type = 1;
eObjectifs[28].realise = 0;
eObjectifs[28].X = 1975;
eObjectifs[28].Y = 1075;
eObjectifs[28].Z = 3;
eObjectifs[28].grabbed = 0;

// 29
eObjectifs[29].type = 1;
eObjectifs[29].realise = 0;
eObjectifs[29].X = 1175;
eObjectifs[29].Y = 925;
eObjectifs[29].Z = 3;
eObjectifs[29].grabbed = 0;

// 30
eObjectifs[30].type = 1;
eObjectifs[30].realise = 0;
eObjectifs[30].X = 1825;
eObjectifs[30].Y = 925;
eObjectifs[30].Z = 3;
eObjectifs[30].grabbed = 0;

// 31
eObjectifs[31].type = 1;
eObjectifs[31].realise = 0;
eObjectifs[31].X = 1175;
eObjectifs[31].Y = 1075;
eObjectifs[31].Z = 3;
eObjectifs[31].grabbed = 0;

// 32
eObjectifs[32].type = 1;
eObjectifs[32].realise = 0;
eObjectifs[32].X = 1825;
eObjectifs[32].Y = 1075;
eObjectifs[32].Z = 3;
eObjectifs[32].grabbed = 0;

// 33
eObjectifs[33].type = 1;
eObjectifs[33].realise = 0;
eObjectifs[33].X = 1025;
eObjectifs[33].Y = 925;
eObjectifs[33].Z = 1;
eObjectifs[33].grabbed = 0;

// 34
eObjectifs[34].type = 1;
eObjectifs[34].realise = 0;
eObjectifs[34].X = 1975;
eObjectifs[34].Y = 925;
eObjectifs[34].Z = 1;
eObjectifs[34].grabbed = 0;

// 35
eObjectifs[35].type = 1;
eObjectifs[35].realise = 0;
eObjectifs[35].X = 1025;
eObjectifs[35].Y = 1075;
eObjectifs[35].Z = 1;
eObjectifs[35].grabbed = 0;

// 36
eObjectifs[36].type = 1;
eObjectifs[36].realise = 0;
eObjectifs[36].X = 1975;
eObjectifs[36].Y = 1075;
eObjectifs[36].Z = 1;
eObjectifs[36].grabbed = 0;

// 37
eObjectifs[37].type = 1;
eObjectifs[37].realise = 0;
eObjectifs[37].X = 1175;
eObjectifs[37].Y = 925;
eObjectifs[37].Z = 1;
eObjectifs[37].grabbed = 0;

// 38
eObjectifs[38].type = 1;
eObjectifs[38].realise = 0;
eObjectifs[38].X = 1825;
eObjectifs[38].Y = 925;
eObjectifs[38].Z = 1;
eObjectifs[38].grabbed = 0;

// 39
eObjectifs[39].type = 1;
eObjectifs[39].realise = 0;
eObjectifs[39].X = 1589;
eObjectifs[39].Y = 300;
eObjectifs[39].Z = 0;
eObjectifs[39].grabbed = 0;

// 40
eObjectifs[40].type = 1;
eObjectifs[40].realise = 0;
eObjectifs[40].X = 1411;
eObjectifs[40].Y = 300;
eObjectifs[40].Z = 0;
eObjectifs[40].grabbed = 0;

// 41
eObjectifs[41].type = 1;
eObjectifs[41].realise = 0;
eObjectifs[41].X = 1100;
eObjectifs[41].Y = 750;
eObjectifs[41].Z = 0;
eObjectifs[41].grabbed = 0;

// 42
eObjectifs[42].type = 1;
eObjectifs[42].realise = 0;
eObjectifs[42].X = 1900;
eObjectifs[42].Y = 750;
eObjectifs[42].Z = 0;
eObjectifs[42].grabbed = 0;

// 43
eObjectifs[43].type = 1;
eObjectifs[43].realise = 0;
eObjectifs[43].X = 1175;
eObjectifs[43].Y = 1075;
eObjectifs[43].Z = 1;
eObjectifs[43].grabbed = 0;

// 44
eObjectifs[44].type = 1;
eObjectifs[44].realise = 0;
eObjectifs[44].X = 1825;
eObjectifs[44].Y = 1075;
eObjectifs[44].Z = 1;
eObjectifs[44].grabbed = 0;

}

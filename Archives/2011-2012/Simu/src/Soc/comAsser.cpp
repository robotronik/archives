#include "comAsser.h"


unsigned char couleurRobotI = COULEUR_ROBOT;
static T_Pos_TabChar posAsser = {{0}};
static int nbCmdInAsser = 0;
FileDepl fileDeplMacro;

void initAsser()
{
    fileDeplMacro.debut = 0;
    fileDeplMacro.taille = 0;
    couleurRobotI = PURPLE;  /// TODO
    if(couleurRobotI == RED) // Rouge
	{
        sendMessageAsser(RED);
	}
    else                    // Violet
	{
        sendMessageAsser(PURPLE);
	}
}

int getNbCmdInAsser()
{
    return nbCmdInAsser;
}

void testAsser()
{
    sendXY_RAPIDE(350, 1750, 10);
	updatePos();
}


void sendXY_RAPIDE(unsigned int x, unsigned int y, unsigned int precision) {
    #ifdef SIMU
    Depl depl;
    depl.typeDepl = DEPL_RAPIDE;
    depl.x = x;
    depl.y = y;
    sendDeplSimu(depl);
    printf("Rapide send : %d %d\n", x, y);
    #endif
    unsigned short xAsser = (x*0xFFFF)/3000;
    if(couleurRobotI == PURPLE)
        xAsser = 0xFFFF - xAsser;
    unsigned short yAsser = ((2000-y)*0xFFFF)/3000;
	UART_Asser_PutChar(XY_RAPIDE);
	UART_Asser_PutChar(xAsser >> 8);
	UART_Asser_PutChar(xAsser);
	UART_Asser_PutChar(yAsser >> 8);
	UART_Asser_PutChar(yAsser);
	precision = 0xFFFF*precision/3000;
	unsigned long p = precision * precision;
	UART_Asser_PutChar(p >> 24);
	UART_Asser_PutChar(p >> 16);
	UART_Asser_PutChar(p >> 8);
	UART_Asser_PutChar(p);
	nbCmdInAsser++;
	signaux.finMouvement = 0;
}

void sendXY_COURBE(unsigned int x, unsigned int y, unsigned int precision) {
    #ifdef SIMU
    Depl depl;
    depl.typeDepl = DEPL_COURBE;
    depl.x = x;
    depl.y = y;
    sendDeplSimu(depl);
    printf("Courbe send : %d %d\n", x, y);
    #endif

	nbCmdInAsser++;
    unsigned short xAsser = (x*0xFFFF)/3000;
    if(couleurRobotI == PURPLE)
        xAsser = 0xFFFF - xAsser;
    unsigned short yAsser = ((2000-y)*0xFFFF)/3000;

	UART_Asser_PutChar(XY_COURBE);
	UART_Asser_PutChar(xAsser >> 8);
	UART_Asser_PutChar(xAsser);
	UART_Asser_PutChar(yAsser >> 8);
	UART_Asser_PutChar(yAsser);
	precision = 0xFFFF*precision/3000;
	unsigned long p = precision * precision;
	UART_Asser_PutChar(p >> 24);
	UART_Asser_PutChar(p >> 16);
	UART_Asser_PutChar(p >> 8);
	UART_Asser_PutChar(p);
	nbCmdInAsser++;
	signaux.finMouvement = 0;
}

void sendALPHA(unsigned int a, unsigned char precision) {
    #ifdef SIMU
    Depl depl;
    depl.typeDepl = DEPL_ALPHA;
    depl.alpha = a;
    sendDeplSimu(depl);
    printf("Alpha send : %d \n", a);
    #endif
	nbCmdInAsser++;

	a = ((a+180)%360) * TOUR / 360;
    if(couleurRobotI == PURPLE)
    	a = (TOUR+(TOUR>>1)-a)%TOUR;
	precision = (unsigned long) precision * TOUR / 360;
	UART_Asser_PutChar(a >> 8);
	UART_Asser_PutChar(a);
	UART_Asser_PutChar(precision);
	nbCmdInAsser++;
	signaux.finMouvement = 0;
}

void sendSUPPR(unsigned char nbCommande) {	// Supprime les N dernières commandes
	UART_Asser_PutChar(SUPPR);
	UART_Asser_PutChar(nbCommande);
	nbCmdInAsser -= nbCommande;
}

void sendFIN_MATCH() {
	UART_Asser_PutChar(FIN_MATCH);
}

void updatePos() {
	UART_Asser_PutChar(GET_POS);
	signaux.posRecue = 0;
}

T_Pos_TabChar getLastPos() {
	T_Pos_TabChar pos;

    unsigned short convertedX = shortLittleToBigEndian(posAsser.x);
    unsigned short convertedY = shortLittleToBigEndian(posAsser.y);
    unsigned short convertedAlpha = shortLittleToBigEndian(posAsser.angle);

	pos.x = (unsigned long) convertedX*3000/0xFFFF;
    pos.y = 2000 - (unsigned short) ((unsigned long) convertedY*3000/0xFFFF);
    if(couleurRobotI == RED)
    {
        pos.angle = (unsigned long) convertedAlpha*360/TOUR;

    }
    else
    {
        pos.x = 3000 - pos.x;
    	pos.angle = (360+180-pos.angle)%360;
	}
	pos.angle = ((180+pos.angle)%360);
	return pos;
}

// Appelees par la fonction d'interruption
void traiterMessageAsser(unsigned char m)
{
	static unsigned char indexPos = 0;
	static char nbOctRestant = 0;

	if (nbOctRestant) {
		nbOctRestant --;
		posAsser.tab[indexPos] = m;	// Un octet Position X 2o ,Y 2o, ALPHA 2o
		indexPos ++; indexPos %= 6;
		if (!nbOctRestant)
		{
			signaux.posRecue = 1;
		}
	}
	else {
		switch(m) {
		case POS:
			nbOctRestant = 6;
			break;
		case DONE :
	    {
            if(isTest())
            {
                // Test reussi
                validerAsser();
                return;
            }
            #ifdef SIMU
            signalAsserDone();
            printf("Done\n");
            #endif
			nbCmdInAsser--;
			signaux.finMouvement = 1;
			break;
	    }
		default :
			//traiterMessageAsser(m); boucle infinie
			break;
		}
	}
}

// Infos
unsigned char isEmptyFileMacro()
{
    return isEmptyFileDepl(fileDeplMacro);
}


static void sendDepl(Depl depl)
{
    switch(depl.typeDepl)
    {
        case DEPL_ALPHA : sendALPHA(depl.alpha, depl.precision); break;
        case DEPL_COURBE : sendXY_COURBE(depl.x, depl.y, depl.precision); break;
        case DEPL_RAPIDE : sendXY_RAPIDE(depl.x, depl.y, depl.precision); break;
        case RECALC :
        {
            // On doit demander a la carto de recalculer
            askChemin(xRobot, yRobot, depl.x, depl.y, getERobotAdverse().X, getERobotAdverse().Y);
            /// TODO AJOUTER TIME OUT
            while(!signaux.pathReceived)
            {
                #ifdef SIMU
                forcerCarto();
                #endif
            }
            // On a recu la carto
            // On ecrit dans la file de macro
            addRecepToDepl(&fileDeplMacro);
            break;
        }
    }

}

void sendDeplGo()
{
    Depl courant;
    while(nbCmdInAsser <= MAX_CMD_IN_ASSER && !isEmptyFileDepl(fileDeplGo))
    {
        if(signaux.blocked)
            break;
        // On recupere la commande
        if(isEmptyFileDepl(fileDeplMacro))
            courant = popFileDepl(&fileDeplGo);
        else
            courant = popFileDepl(&fileDeplMacro);
        sendDepl(courant);
    }
}

void sendDeplReturn()
{
    Depl courant;
    while(nbCmdInAsser <= MAX_CMD_IN_ASSER && !isEmptyFileDepl(fileDeplReturn))
    {
        if(signaux.blocked)
            break;
        // On recupere la commande
        if(isEmptyFileDepl(fileDeplMacro))
            courant = popFileDepl(&fileDeplReturn);
        else
            courant = popFileDepl(&fileDeplMacro);
        sendDepl(courant);
    }
}

void stopAsserByBrake()
{
    /// TODO : +1 au cas ou ?
    if(nbCmdInAsser > 0)
        sendSUPPR(nbCmdInAsser);
    // Vider les 3 files
    viderFile(&fileDeplGo);
    viderFile(&fileDeplMacro);
    viderFile(&fileDeplReturn);
}

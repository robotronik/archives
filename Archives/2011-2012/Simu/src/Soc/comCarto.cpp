#include "comCarto.h"

static unsigned char id = 0;
static Cache cache;
FileDepl fileRecepCarto;
static int lastXa = 0;
static int lastYa = 0;

void initComCarto()
{
    cache.courant = 0;

    cache.v[0].x = 1100;
    cache.v[0].y = 1500;
    cache.v[0].longueur = 1024;
    cache.v[8].longueur = 1824;

    cache.v[1].x = 1900;
    cache.v[1].y = 1500;
    cache.v[1].longueur = 1724;
    cache.v[9].longueur = 924;

    cache.v[2].x = 1100;
    cache.v[2].y = 800;
    cache.v[2].longueur = 1923;
    cache.v[10].longueur = 2623;

    cache.v[3].x = 1900;
    cache.v[3].y = 800;
    cache.v[3].longueur = 2723;
    cache.v[11].longueur = 1823;

    cache.v[4].x = 500;
    cache.v[4].y = 400;
    cache.v[4].longueur = 1982;
    cache.v[12].longueur = 3130;

    cache.v[5].x = 1500;
    cache.v[5].y = 400;
    cache.v[5].longueur = 2347;
    cache.v[13].longueur = 2247;

    cache.v[6].x = 2500;
    cache.v[6].y = 400;
    cache.v[6].longueur = 3230;
    cache.v[14].longueur = 1882;

    cache.v[7].x = 2700;
    cache.v[7].y = 1100;
    cache.v[7].longueur = 2788;
    cache.v[15].longueur = 1265;

    int i;
    for(i = 0; i<7; i++)
    {
        cache.v[i+8].x = cache.v[i].x;
        cache.v[i+8].y = cache.v[i].y;
    }

}


void startRefreshCarto()
{
    askLongueurChemin(xRobot, yRobot, cache.v[0].x, cache.v[0].y, eRobotAdverse.X, eRobotAdverse.Y);
}

unsigned int getChemin(int xa, int ya, char adversaire)
{
    unsigned int debut = (adversaire) ? 8 : 0;
    unsigned int fin = (adversaire) ? 16 : 8;
    unsigned int bestLength = NSQ((cache.v[debut].x-xa)/10, (cache.v[debut].y-ya)/10);
    unsigned int buffLength;
    unsigned int best = debut;
    int i;
    for(i=debut+1; i<fin; i++)
    {
        buffLength = NSQ((cache.v[i].x-xa)/10, (cache.v[i].y-ya)/10);
        if(buffLength < bestLength)
        {
            bestLength = buffLength;
            best = i;
        }
    }

    return cache.v[best].longueur;
}

// Appele par le traitant
void traiterMessageCarto(unsigned char m)
{
    // Reconstruction de la trame
    static Trame t;
    static unsigned char i = 0;
    switch(i)
    {
        case 0 : t.charx4.Xooo = m; break;
        case 1 : t.charx4.oXoo = m; break;
        case 2 : t.charx4.ooXo = m; break;
        case 3 : t.charx4.oooX = m; break;
        default : break;
    }
    i++;
    if(i==4)
    {
        i = 0;
        traiterTrame(t);
    }

}

void traiterTrame(Trame t)
{
    if(isTest())
    {
        // Test reussi si la valeur vaut approximativement 2806
		unsigned int longueur = t.first.longueur;
		if(longueur > 2706 && longueur < 2906)
        	validerCarto();
        return;
    }
    // Update de chemin
    if(t.first.numero == 0b111111)
    {
        cache.v[cache.courant].longueur = (t.first.nb == 0) ? -1 : t.first.longueur;
        cache.courant++;
        cache.courant%=TAILLE_CACHE;

        // On envoie le prochain calcul a la carto
        // DELAY ?
        if(cache.courant<=7) // From Us
        {
            askLongueurChemin(xRobot, yRobot, cache.v[cache.courant].x, cache.v[cache.courant].y, eRobotAdverse.X, eRobotAdverse.Y);

        }
        else // From him
        {
            askLongueurChemin(eRobotAdverse.X, eRobotAdverse.Y, cache.v[cache.courant].x, cache.v[cache.courant].y, xRobot, yRobot);
        }

    }
    // Chemin recu
    else
    {
        static char debut = 1;
        static char filePleineDone = 0;
        static int tramesRestantes = 0;
        if(debut)
        {
            // On verifie que la trame est bien une trame de debut
            if(t.first.code != 0b1011)
            {
                // Si ce n'est pas le cas, on renvoie la commande
                renvoyerDerniereCommande();
                return;
            }
            // On place debut a 0
            debut = 0;
            // On reinit filePleineDone
            filePleineDone = 0;
            // On vide la file de reception
            viderFile(&fileRecepCarto);
            // si la longueur est nulle
            if(t.first.longueur == 0)
            {
                // On place fin de reception
                signaux.pathReceived = 1;
                // On est bloque
                signaux.blocked = 1;
                return;
            }
            else
            {
                // on change la valeur de trames Restantes
                tramesRestantes = t.first.nb;
                // Si tramesRestantes est nulle a ce stade, on est deja au bon endroit
                if(tramesRestantes == 0)
                {
                    signaux.pathReceived = 1;
                    debut = 1;
                }
            }

        }
        else
        {
            // On verifie que c'est bien une trame de chemin
            if(t.data.code != 0b010)
            {
                // Si ce n'est pas un point de passage car on aurait eu une erreur de nb de points de passage
                if(!signaux.pathReceived)
                {
                    // Si ce n'est pas le cas, on renvoie la commande et on place debut a 0
                    renvoyerDerniereCommande();
                    debut = 1;
                    // On vide la file de reception
                    viderFile(&fileRecepCarto);
                    // le path n'est pas recu
                    signaux.pathReceived = 0;
                }
                return;
            }
            // Si la file n'est pas pleine
            if(fileRecepCarto.taille < NB_DEPL_MAX-1)
            {
                // On ajoute la trame
                Depl buff;
                buff.typeDepl = DEPL_RAPIDE; // DEPL_COURBE;
                buff.x = t.data.x;
                buff.y = t.data.y;
                pushFileDepl(&fileRecepCarto, buff);
                if(tramesRestantes == 1)    // Plus precis sur le dernier
                    buff.precision = PRECISION_DEFAULT_PRECIS;
                else
                    buff.precision = PRECISION_CARTO;

            }
            else
            {
                // Si pas filePleineDone
                if(!filePleineDone)
                {
                    // on place une macro
                    Depl buff;
                    buff.typeDepl = RECALC;
                    buff.x = lastXa;
                    buff.y = lastYa;
                    pushFileDepl(&fileRecepCarto, buff);
                    filePleineDone = 1;
                }
            }
            // On decremente tramesRestantes
            tramesRestantes--;
            // Si on a plus de trames Restantes
            if(!tramesRestantes)
            {
                // On replace debut a 1
                debut = 1;
                // On place fin de reception
                signaux.pathReceived = 1;
            }

        }

    }
}


void askChemin(unsigned int xd, unsigned int yd, unsigned int xa, unsigned int ya, unsigned int xAdv, unsigned int yAdv)
{
    signaux.pathReceived = 0;
    lastXa = xa;
    lastYa = ya;

    Trame t;

    // Trame Depart
    t.data.code = 0b111;
    t.data.x = xd;
    t.data.y = yd;
    t.data.numero = id;
    sendMessageCarto(t.charx4.Xooo);
    sendMessageCarto(t.charx4.oXoo);
    sendMessageCarto(t.charx4.ooXo);
    sendMessageCarto(t.charx4.oooX);

    // Trame Arrivee
    t.data.code = 0b101;
    t.data.x = xa;
    t.data.y = ya;
    sendMessageCarto(t.charx4.Xooo);
    sendMessageCarto(t.charx4.oXoo);
    sendMessageCarto(t.charx4.ooXo);
    sendMessageCarto(t.charx4.oooX);

    // Trame Robot Adverse
    t.data.code = 0b011;
    t.data.x = xAdv;
    t.data.y = yAdv;
    sendMessageCarto(t.charx4.Xooo);
    sendMessageCarto(t.charx4.oXoo);
    sendMessageCarto(t.charx4.ooXo);
    sendMessageCarto(t.charx4.oooX);

	#ifdef DEUX_ROBOTS_ADVERSES
    // Trame Robot Adverse 2
    t.data.code = 0b011;
    t.data.x = xAdv;
    t.data.y = yAdv;
    sendMessageCarto(t.charx4.Xooo);
    sendMessageCarto(t.charx4.oXoo);
    sendMessageCarto(t.charx4.ooXo);
    sendMessageCarto(t.charx4.oooX);
	#endif

    id++;
}

void askLongueurChemin(unsigned int xd, unsigned int yd, unsigned int xa, unsigned int ya, unsigned int xAdv, unsigned int yAdv)
{
    Trame t;

    // Trame Depart
    t.data.code = 0b110;
    t.data.x = xd;
    t.data.y = yd;
    t.data.numero = 0b111111;
    sendMessageCarto(t.charx4.Xooo);
    sendMessageCarto(t.charx4.oXoo);
    sendMessageCarto(t.charx4.ooXo);
    sendMessageCarto(t.charx4.oooX);

    // Trame Arrivee
    t.data.code = 0b100;
    t.data.x = xa;
    t.data.y = ya;
    sendMessageCarto(t.charx4.Xooo);
    sendMessageCarto(t.charx4.oXoo);
    sendMessageCarto(t.charx4.ooXo);
    sendMessageCarto(t.charx4.oooX);

    // Trame Robot Adverse
    t.data.code = 0b010;
    t.data.x = xAdv;
    t.data.y = yAdv;
    sendMessageCarto(t.charx4.Xooo);
    sendMessageCarto(t.charx4.oXoo);
    sendMessageCarto(t.charx4.ooXo);
    sendMessageCarto(t.charx4.oooX);


	#ifdef DEUX_ROBOTS_ADVERSES
    // Trame Robot Adverse 2
    t.data.code = 0b011;
    t.data.x = xAdv;
    t.data.y = yAdv;
    sendMessageCarto(t.charx4.Xooo);
    sendMessageCarto(t.charx4.oXoo);
    sendMessageCarto(t.charx4.ooXo);
    sendMessageCarto(t.charx4.oooX);
	#endif
}

void renvoyerDerniereCommande()
{
    askChemin(xRobot, yRobot, lastXa, lastYa, getERobotAdverse().X, getERobotAdverse().Y);
}

// Fonctions de file

void pushFileDepl(FileDepl* file, Depl entree)
{
    file->file[(file->debut+file->taille)%NB_DEPL_MAX] = entree;
    file->taille++;
}

Depl popFileDepl(FileDepl* file)
{
    unsigned char i = file->debut;
    file->debut = (file->debut + 1)%NB_DEPL_MAX;
    file->taille--;
    return file->file[i];
}

char isEmptyFileDepl(FileDepl file)
{
    return (file.taille == 0);
}

char isFullFileDepl(FileDepl file)
{
    return (file.taille == NB_DEPL_MAX);
}

void viderFile(FileDepl* file)
{
    file->debut = 0;
    file->taille = 0;
}

void copierFile(FileDepl* dest, FileDepl source)
{
    int i;
    for(i=0; i<source.taille; i++)
    {
        dest->file[(source.debut+i)%NB_DEPL_MAX] = source.file[(source.debut+i)%NB_DEPL_MAX];
    }
    dest->debut = source.debut;
    dest->taille = source.taille;
}

void addRecepToDepl(FileDepl* depl)
{
    int i = 1;
    Depl buff;
    while(!isEmptyFileDepl(fileRecepCarto))
    {
        // Trop de points de passages
        if(i == NB_RECEP_MAX)
        {
            // On ajoute une macro
            buff.typeDepl = RECALC;
            buff.x = lastXa;
            buff.y = lastYa;
            pushFileDepl(depl, buff);
            return;
        }
        buff = popFileDepl(&fileRecepCarto);
        if(i != 1)  // On n'envoie pas le premier
            pushFileDepl(depl, buff);

        i++;
    }
}

void testCarto()
{
    // On demande un chemin tout con
    askLongueurChemin(250, 1750, 2000, 500, 2750, 1750);
}

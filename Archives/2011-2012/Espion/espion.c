#include <stdio.h>
#include <stdio.h>
#include "espion.h"
#include "affich.h"


void afficheObjectif(Objectifs *obj)
{
    Objectifs *p;
    p=obj;
    while(p != NULL)
    {
        drawDisque(p->X, p->Y, 10,0,0,0);
        p=p->suiv;
    }

}


void espion( Objectifs *obj, ERobot robot1, ERobot robot2)//Regarde si un des robots adverse prend un objectif.
{
	Objectifs *p = obj;

	while(p != NULL)
	{
		if( prendObjectif(p->X,p->Y, robot1))
		{
			robot1.nbPoints += p->nbPoints;
			//Supression de l'objectif
			p->suiv = (p->suiv)->suiv;

		}
		else if(  prendObjectif(p->X,p->Y, robot2))
		{
			robot2.nbPoints += p->nbPoints;
			//Supression de l'objectif
			p->suiv = (p->suiv)->suiv;

		}
		p = p->suiv;
	}

}

unsigned short int prendObjectif(unsigned int X, unsigned int Y, ERobot rb )//Si le robot adverse est en position dep rendre un objectif
{
	if((X-rb.X)*(X-rb.X) + (Y-rb.Y)*(Y-rb.Y) < DISTANCE_MIN2)
	{
		return 1;
	}
	return 0;
}


void supprimeObjectif(Objectifs *obj, unsigned int id)
{
	Objectifs *p = obj;
	while(p != NULL && p->id != id)
	{
		p = p->suiv;
	}
	if(p!= NULL && p->id == id)
	{
        Objectifs *q; q = p->suiv;
        free(p->suiv);
		p->suiv = q->suiv;
	}
}

void ajoutObjectif(Objectifs *obj,unsigned int X, unsigned int Y, unsigned int nbPoints, char type)
{
	Objectifs *p=obj;
	while(p->suiv != NULL)
	{
		p=p->suiv;
	}

	p->suiv=   calloc(1, sizeof(Objectifs));
	if( p->suiv!= NULL)
	{
		p=p->suiv;
		p->X = X;
		p->Y=Y;
		p->nbPoints = nbPoints;
		p->type = type;
	}
}






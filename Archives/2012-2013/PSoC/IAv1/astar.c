#include "astar.h"
#include "malloc.h"
#include "constantes.h"
#include <stdlib.h>

extern char listEnable[NB_POINTS];
extern int erreurAlloc;

/*void addListeFils(ListeFils* l, int id)
{
    ListeFils nouveau = my_malloc(sizeof(struct _ListeFils));
        nouveau->id = id;
        nouveau->queue = *l;
    *l = nouveau;
}*/

void addSet(Set* l, int id)
{
    Set nouveau = my_malloc2(sizeof(struct _Set));

        nouveau->id = id;

        nouveau->queue = *l;
    *l = nouveau;
}
Set removeIdSet(Set liste, int valeur)
{

    if(liste == NULL)
        return NULL;

    if(liste->id == valeur)
    {

        Set  tmp = liste->queue;
        //my_free(liste);

        tmp = removeIdSet(tmp, valeur);
        return tmp;
    }
    else
    {
        liste->queue = removeIdSet(liste->queue, valeur);
        return liste;
    }
}

int isInSet(Set list , int id)
{
    Set p = list;
    while (p != NULL)
    {
        if ( p->id == id )
        {
            return 1;
        }
        p = p->queue;
    }
    return 0;
}

Data setGscore(int id , int val , Data l)
{
    Data p = l;
    while ( p != NULL )
    {
        if ( p->id == id)
        {
            p->g = val;
            return l;
        }
        p = p->queue;
    }

    Data nouveau = my_malloc2(sizeof( struct _Data));
    nouveau->id = id;
    nouveau->g =val;
    nouveau->queue = l;
    return nouveau;
}

int getGscore(int id , Data l)
{

    Data p = l;
    while ( p != NULL)
    {
        if ( p->id == id)
        {
            return p->g;
        }
        p = p->queue;
    }
    return -1;
}

Data setHscore(int id , int val , Data l)
{
    Data p = l;
    while ( p != NULL )
    {
        if ( p->id == id)
        {
            p->h = val;
            return l;
        }
        p = p->queue;
    }

    Data nouveau = my_malloc2(sizeof( struct _Data));
    nouveau->id = id;
    nouveau->h =val;
    nouveau->queue = l;
    return nouveau;
}

int getHscore(int id , Data l)
{

    Data p = l;
    while ( p != NULL)
    {
        if ( p->id == id)
        {
            return p->h;
        }
        p = p->queue;
    }
    return -1;
}

Data setFscore(int id , int val , Data l)
{
    Data p = l;
    while ( p != NULL )
    {
        if ( p->id == id)
        {
            p->f = val;
            return l;
        }
        p = p->queue;
    }

    Data nouveau = my_malloc2(sizeof( struct _Data));
    nouveau->id = id;
    nouveau->f =val;
    nouveau->queue = l;
    return nouveau;
}

int getFscore(int id , Data l)
{

    Data p = l;
    while ( p != NULL)
    {
        if ( p->id == id)
        {
            return p->f;
        }
        p = p->queue;
    }
    return -1;
}

Came setCameFrom( int id , int val , Came l)
{
    Came p = l;
    while ( p != NULL )
    {
        if ( p->id == id)
        {
            p->valIdCame = val;
            return l;
        }
        p = p->queue;
    }

    Came nouveau = my_malloc2(sizeof( struct _Came));
    nouveau->id = id;
    nouveau->valIdCame =val;
    nouveau->queue = l;
    return nouveau;
}

int getCameFrom(int id , Came l)
{
    Came p = l;
    while ( p != NULL)
    {
        if ( p->id == id)
            return p->valIdCame;

        p = p->queue;
    }
    return -1;//pas d�fini
}

void addCoor(Coor* l, int id ,int x,int y)
{
    if ( *l != NULL && id == (*l)->id)
    {
        return;
    }

    Coor nouveau = my_malloc2(sizeof(struct _Coor));
        nouveau->x = x;
        nouveau->y = y;
        nouveau->id = id;
        nouveau->queue = *l;
    *l = nouveau;
}

int nbCoor(Coor list)
{
    int i = 0;
    while (list != NULL)
     {
         i++;
         list = list->queue;
     }
     return i;
}

Coor removeIdCoor(Coor liste, int valeur)
{

    if(liste == NULL)
        return NULL;

    if(liste->id == valeur)
    {

        Coor  tmp = liste->queue;
        //my_free(liste);

        tmp = removeIdCoor(tmp, valeur);
        return tmp;
    }
    else
    {
        liste->queue = removeIdCoor(liste->queue, valeur);
        return liste;
    }
}


int distBetween( int id1 ,int id2)
{
    if ( id2 > 1000 )
        return MAILLE*RACINE_2;
    else
        return MAILLE;
}

int idToXy(PointROM * list, int id, int* x,int* y)
{
    //Point p = list;
    //while ( p != NULL)
    //{
        if ( list[id-1].id == id)
        {
            *x = list[id-1].x;
            *y = list[id-1].y;
            return 1;
        }

       // p = p->queue;
    //}
    return -1;
}


int heuristicCostEstimate( int id1 , int id2 ,  PointROM * list)
{
    int x1,y1,x2,y2;
    idToXy(list,id1, &x1, &y1);
    idToXy(list,id2, &x2, &y2);

    //return  sqrt(abs(x2-x1)*abs(x2-x1) + abs(y2-y1)*abs(y2-y1) );
    return (abs(x2-x1)+ abs(y2-y1));
}

int lowestFscore(Set openSet, Data data)
{
    int idmin = openSet->id;
    int fmin = getFscore(idmin,data);
    Set p = openSet;
    while ( p != NULL)
    {
        if ( getFscore(p->id , data) < fmin)
        {
            idmin = p->id;
            fmin = getFscore(idmin, data);
        }
        p = p->queue;
    }
    return idmin;

}

Point sePlacerEn(int id , PointROM * list)
{
//    Point p = list;
//    while ( p != NULL)
//    {
//        if ( p->id == id)
//        {
//            return p;
//        }
     //   p = p->queue;
    //}
    return list[id-1];
}

Set reconstruireChemin(Came l,int startId ,int goalId)
{
    Set p = NULL;
    int x;
    addSet(&p,goalId);
    x = getCameFrom(goalId,l);
    addSet(&p,x);
	if (erreurAlloc)
		return p;
    while ( x != startId)
    {
        x = getCameFrom(x,l);
        addSet(&p,x);
		if (erreurAlloc)
			return p;
    }
    return p;
}

Coor setToCoor(Set list , PointROM * points)
{
    Set p = list;
    Coor res = NULL;
    int x,y,id;
    while ( p != NULL)
    {
        id = p->id;
        idToXy(points,id,&x,&y);
        addCoor(&res, id,x,y);
		if (erreurAlloc)
			return res;
        p = p->queue;
    }
    return res;
}

int testAligne(int xAv,int yAv,int xMil,int yMil,int xAp,int yAp)
{
    if (xMil == xAv && xMil == xAp)
        return 1;
    else if (yMil == yAv && yMil == yAp)
        return 1;
    else if (xAp == xMil+MAILLE && xAv == xMil-MAILLE && yAp == yMil+MAILLE && yAv == yMil-MAILLE)
        return 1;
    else if (xAp == xMil-MAILLE && xAv == xMil+MAILLE && yAp == yMil+MAILLE && yAv == yMil-MAILLE)
        return 1;
    else if (xAp == xMil+MAILLE && xAv == xMil-MAILLE && yAp == yMil-MAILLE && yAv == yMil+MAILLE)
        return 1;
    else if (xAp == xMil-MAILLE && xAv == xMil+MAILLE && yAp == yMil-MAILLE && yAv == yMil+MAILLE)
        return 1;

    return 0;
}

Coor simplifierResultat(Coor list)
{
    if (list== NULL || list->queue == NULL)
        return list;
    Coor p = list;
    int idAv,idMil,idAp;
    int xAv,yAv,xMil,yMil,xAp,yAp;
    idAv = p->id;
    xAv = p->x;
    yAv = p->y;
    p = p->queue;
    idMil = p->id;
    xMil = p->x;
    yMil = p->y;
    p = p->queue;

    while ( p!= NULL)
    {
        idAp = p->id;
        xAp = p->x;
        yAp = p->y;

        if ( testAligne(xAv,yAv,xMil,yMil,xAp,yAp))
        {
            removeIdCoor(list,idMil);
        }
        idAv = idMil;
        xAv = xMil;
        yAv = yMil;
        idMil = idAp;
        xMil = xAp;
        yMil = yAp;

        p = p->queue;
    }
    return list;
}

#ifdef DEUX_ROBOT_ADV
void loadAdversaire(int x, int y, int x2, int y2, PointROM * list)
#else
void loadAdversaire(int x, int y, PointROM * list)
#endif
{
    int i;
    if (x == -1 && y == -1)
    {
        for (i = 0 ; i < NB_POINTS ; i++)
        {
            //list[i].enable = 3;
            listEnable[i] = 3;
        }
        return;
    }
    for (i = 0 ; i < NB_POINTS ; i++)
    {
        if ( list[i].x <= x+(TAILLE_ADV+TAILLE_ROBOT)/2 && list[i].x >= x-(TAILLE_ADV+TAILLE_ROBOT)/2 && list[i].y <= y+(TAILLE_ROBOT+TAILLE_ADV)/2 && list[i].y >= y-(TAILLE_ROBOT + TAILLE_ADV)/2)
        {
            //list[i].enable = 0;
            listEnable[i] = 0;
        }
        else if ( list[i].x <= x+3*TAILLE_ADV/2 && list[i].x >= x-3*TAILLE_ADV/2 && list[i].y <= y+3*TAILLE_ADV/2 && list[i].y >= y-3*TAILLE_ADV/2)
        {
            //list[i].enable = 1;
            listEnable[i] = 1;
        }
        else if ( list[i].x <= x+2*TAILLE_ROBOT && list[i].x >= x-2*TAILLE_ROBOT && list[i].y <= y+2*TAILLE_ROBOT && list[i].y >= y-2*TAILLE_ROBOT)
        {
            //list[i].enable = 2;
            listEnable[i] = 2;
        }
        #ifdef DEUX_ROBOT_ADV
        if ( list[i].x <= x2+(TAILLE_ADV2+TAILLE_ROBOT)/2 && list[i].x >= x2-(TAILLE_ADV2+TAILLE_ROBOT)/2 && list[i].y <= y2+(TAILLE_ROBOT+TAILLE_ADV2)/2 && list[i].y >= y2-(TAILLE_ROBOT + TAILLE_ADV2)/2)
        {
            //list[i].enable = 0;
            listEnable[i] = 0;
        }
        else if ( list[i].x <= x2+3*TAILLE_ADV2/2 && list[i].x >= x2-3*TAILLE_ADV2/2 && list[i].y <= y2+3*TAILLE_ADV2/2 && list[i].y >= y2-3*TAILLE_ADV2/2)
        {
            //list[i].enable = 1;
            listEnable[i] = 1;
        }
        else if ( list[i].x <= x2+2*TAILLE_ROBOT && list[i].x >= x2-2*TAILLE_ROBOT && list[i].y <= y2+2*TAILLE_ROBOT && list[i].y >= y2-2*TAILLE_ROBOT)
        {
            //list[i].enable = 2;
            listEnable[i] = 2;
        }
        #endif
		else
		{
			//list[i].enable = 3;
			listEnable[i] = 3;
		}

        //p = p->queue;
    }
    return;
}

int calculLongueur(Coor list)
{
	Coor p = list;
	if (p == NULL)
		return 0;
	int longueur = 0;
	int x,y,xav,yav;
	
	xav = p->x;
	yav = p->y;
	p = p->queue;
	
	while ( p != NULL ) 
	{
		x = p->x;
		y = p->y;
		if ( x == xav )
			longueur = longueur + abs(y-yav);//vertical
		else if ( y == yav )
			longueur = longueur + abs(x-xav);//horizontal
		else
			longueur = longueur + abs(x-xav)*1.414;//diagonal

		p=p->queue;
		xav = x;
		yav = y;
	}
	return longueur;
}

void calculDepartArrivee(PointROM * list,int xDep,int yDep,int xArr,int yArr,int* startId,int* goalId) //en norme 1
{
    Point p;
    int distDep,distArr,distMinDep,distMinArr,idMinDep,idMinArr;
    int i = 0;
    while ( i != NB_POINTS-1 && /*list[i].enable != 3*/listEnable[i] != 3 )
    {
	    i++;
    }
    p = list[i];

    distMinDep = 100*(abs(xDep-p.x) + abs(yDep-p.y) )+ abs(p.x-xArr) + abs(p.y-yArr);
    distMinArr = abs(xArr-p.x) + abs(yArr-p.y);
    idMinDep = p.id;
    idMinArr = p.id;
    p = list[p.id];

    while(p.id < NB_POINTS)
    {
        distDep = 100*(abs(xDep - p.x) + abs(yDep - p.y)) + abs(p.x-xArr) + abs(p.y-yArr);
        distArr = abs(xArr - p.x) + abs(yArr - p.y);

        if (distDep < distMinDep)
        {
            if (/*p.enable*/listEnable[p.id-1] != 0)
            {
                idMinDep = p.id;
                distMinDep = distDep;
            }
        }

        if (distArr < distMinArr)
        {
            if (/*p.enable*/listEnable[p.id-1]  != 0)
            {
                idMinArr = p.id;
                distMinArr = distArr;
            }
        }
        p = list[p.id];
        //p = p->queue;
    }
    *startId = idMinDep;
    *goalId  = idMinArr;
}

Coor inverser(Coor list)
{
	Coor l = list;
	Coor r = NULL;
	Coor p = NULL;
	while (l != NULL)
	{
		p = l;
		l = l->queue;
		p->queue = r;
		r =  p;
	}
	return r;		
}	

Coor aStar(int xDep, int yDep , int xArr, int yArr, PointROM * list)
{
	
    int startId,goalId;
    calculDepartArrivee(list,xDep,yDep,xArr,yArr,&startId,&goalId);///
    int danger;

    int x,y,yr;//varialble d'id pas de coordon�es
    int tentativeGscore,tentativeIsBetter;

    Point p;
    int f = 0;

    Set presqueResultat = NULL;
    Coor resultat = NULL;

    Set closedSet = NULL;
    Set openSet = NULL;
    addSet(&openSet , startId);

    Came cameFrom = NULL;

    Data data = NULL;

    data = setGscore(startId, 0 ,data);
    data = setHscore(startId, heuristicCostEstimate(startId,goalId,list),data);
    data = setFscore(startId, data->g + data->h, data);

    while (openSet != NULL)
    {
        x = lowestFscore(openSet,data);
        if ( x == goalId)
        {
            presqueResultat = reconstruireChemin(cameFrom,startId,goalId);
			if (testAlloc() == -1)
				return NULL;
            resultat = setToCoor(presqueResultat,list);
			if (testAlloc() == -1)
				return NULL;
            resultat = simplifierResultat(resultat);

            addCoor(&resultat,0,xArr,yArr);
			if (testAlloc() == -1)
				return NULL;
			resultat = inverser(resultat);
            return resultat;
        }


        openSet = removeIdSet(openSet,x);
        addSet(&closedSet, x);
		
		/* test malloc */
		if (testAlloc() == -1)
			return NULL;

        p = sePlacerEn(x,list);

		for (f = 0 ; p.lPt[f] != -1 && f < NB_FILS ; f++)//pour tout les y voisins de x
        {
            y = p.lPt[f];
            yr = y;
            if ( y > 1000)
            {
                y = y-1000;
            }


            if ( isInSet(closedSet,y) ||listEnable[p.id-1] /*p.enable*/ == 0)
            {
                continue;
            }
            tentativeGscore = getGscore(x,data) + distBetween(x,yr);
            if ( isInSet(openSet,y) == 0)
            {
                addSet(&openSet,y);
				if (testAlloc() == -1)
					return NULL;
                data = setHscore(y,heuristicCostEstimate(y,goalId,list),data);
                tentativeIsBetter = 1;
				if (testAlloc() == -1)
					return NULL;
            }
            else if (tentativeGscore <= getGscore(y,data))
            {
                tentativeIsBetter = 1;
            }
            else
            {
                tentativeIsBetter = 0;
            }

            if ( tentativeIsBetter == 1)
            {
                cameFrom = setCameFrom( y , x , cameFrom);
				if (testAlloc() == -1)
					return NULL;
                data = setGscore(y,tentativeGscore,data);
				if (testAlloc() == -1)
					return NULL;
                 danger =listEnable[p.id-1] ;
                if (danger == 3)
                    danger = 0;
                else if (danger == 2)
                    danger = 100;
                else if (danger  == 1)
                    danger = 500;
                data = setFscore(y,getGscore(y,data) + getHscore(y,data) + danger, data);
				if (testAlloc() == -1)
					return NULL;
            }
        }
    }
    return NULL;// = erreur
}

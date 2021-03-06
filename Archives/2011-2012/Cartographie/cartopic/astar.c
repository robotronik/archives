#include "astar.h"

void addListeFils(ListeFils* l, int id)
{
    ListeFils nouveau = my_malloc(sizeof(struct ListeFils));
        nouveau->id = id;
        nouveau->queue = *l;
    *l = nouveau;
    //*l = NULL;
}

void suppListeFils(ListeFils list)
{
    ListeFils buff;
    while(list != NULL)
    {
        buff = list->queue;
        //my_free(list);
        list = buff;
    }
}

void addPoint(Point* l, int x,int y, int id, ListeFils lPt)
{
    //unsigned long taille = sizeof(struct Point);
    Point nouveau = my_malloc(sizeof(struct Point));
        nouveau->x = x;
        nouveau->y = y;
        nouveau->id = id;
        nouveau->lPt = lPt;
        nouveau->queue = *l;
    *l = nouveau;
}

void suppPoint(Point list)
{
    Point buff;
    while(list != NULL)
    {
        buff = list->queue;
        //my_free(list);
        list = buff;
    }
}

void addSet(Set* l, int id)
{
    Set nouveau = my_malloc2(sizeof(struct Set));

        nouveau->id = id;

        nouveau->queue = *l;
    *l = nouveau;
}

void suppSet(Set list)
{
    Set buff;
    while(list != NULL)
    {
        buff = list->queue;
        //my_free(list);
        list = buff;
    }
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

void suppData(Data list)
{
    Data buff;
    while(list != NULL)
    {
        buff = list->queue;
        //my_free(list);
        list = buff;
    }
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

    Data nouveau = my_malloc2(sizeof( struct Data));
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

    Data nouveau = my_malloc2(sizeof( struct Data));
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

    Data nouveau = my_malloc2(sizeof( struct Data));
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

    Came nouveau = my_malloc2(sizeof( struct Came));
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

void suppCame(Came list)
{
    Came buff;
    while(list != NULL)
    {
        buff = list->queue;
        //my_free(list);
        list = buff;
    }
}

void addCoor(Coor* l, int id ,int x,int y)
{
    if ( *l != NULL && id == (*l)->id)
    {
        return;
    }

    Coor nouveau = my_malloc2(sizeof(struct Coor));
        nouveau->x = x;
        nouveau->y = y;
        nouveau->id = id;
        nouveau->queue = *l;
    *l = nouveau;
}

void suppCoor(Coor list)
{
    Coor buff;
    while(list != NULL)
    {
        buff = list->queue;
        //my_free(list);
        list = buff;
    }
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
        return MAILLE*1.414;
    else
        return MAILLE;
}

int idToXy(Point list, int id, int* x,int* y)
{
    Point p = list;
    while ( p != NULL)
    {
        if ( p->id == id)
        {
            *x = p->x;
            *y = p->y;
            return 1;
        }

        p = p->queue;
    }
    return -1;
}

int abs(int a)
{
    if ( a>=0 )
        return a;
    else
        return -a;
}

int heuristicCostEstimate( int id1 , int id2 ,  Point list)
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

Point sePlacerEn(int id , Point list)
{
    Point p = list;
    while ( p != NULL)
    {
        if ( p->id == id)
        {
            return p;
        }
        p = p->queue;
    }
    return list;
}

Set reconstruireChemin(Came l,int startId ,int goalId)
{
    Set p = NULL;
    int x;
    addSet(&p,goalId);
    x = getCameFrom(goalId,l);
    addSet(&p,x);
    while ( x != startId)
    {
        x = getCameFrom(x,l);
        addSet(&p,x);
    }
    return p;
}

Coor setToCoor(Set list , Point points)
{
    Set p = list;
    Coor res = NULL;
    int x,y,id;
    while ( p != NULL)
    {
        id = p->id;
        idToXy(points,id,&x,&y);
        addCoor(&res, id,x,y);

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

Coor aStar(int startId , int goalId, Point list)
{
    int x,y,yr;//varialble d'id pas de coordon�es
    int tentativeGscore,tentativeIsBetter;

    Point p =NULL;
    ListeFils f = NULL;

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
            resultat = setToCoor(presqueResultat,list);
            resultat = simplifierResultat(resultat);
            /*suppSet(presqueResultat);
            suppData(data);
            suppCame(cameFrom);
            suppSet(closedSet);
            suppSet(openSet);*/
            return resultat;
        }


        openSet = removeIdSet(openSet,x);
        addSet(&closedSet, x);


        p = sePlacerEn(x,list);
        for ( f = p->lPt ; f != NULL ; f = f->queue )//pour tout les y voisins de x
        {
            y = f->id;
            yr = y;
            if ( y > 1000)
            {
                y = y-1000;
            }


            if ( isInSet(closedSet,y) /*|| p->enable == 0*/)
            {
                continue;
            }
            tentativeGscore = getGscore(x,data) + distBetween(x,yr);
            if ( isInSet(openSet,y) == 0)
            {
                addSet(&openSet,y);
                data = setHscore(y,heuristicCostEstimate(y,goalId,list),data);
                tentativeIsBetter = 1;
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
                data = setGscore(y,tentativeGscore,data);
                data = setFscore(y,getGscore(y,data) + getHscore(y,data), data);
            }




        }
    }
    return NULL;// = erreur
}

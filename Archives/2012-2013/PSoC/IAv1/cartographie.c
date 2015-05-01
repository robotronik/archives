/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "cartographie.h"
#include "astar.h"
#include "malloc.h"
#include "data.h"
#include "constantes.h"
#include "deplacement.h"


PointROM * list = NULL;
Coor chemin = NULL;

extern unsigned char* heap_ptr;
void initCarto(void)
{
	my_malloc_init();
	list = loadPointList();

}


int genererEvitementAstar(Action * tab, Pos pDep, Pos pArr, Pos pObst, int * status)
{
	int i= 0;
	Pos pos;
	Coor p = NULL;
	PRINTF("dx:%d dy:%d, ax:%d ay:%d, ox:%d oy:%d\r",pDep.x,pDep.y,pArr.x,pArr.y,pObst.x,pObst.y);
	my_free();
	loadAdversaire(pObst.x,pObst.y,list);
	chemin = aStar(pDep.x, pDep.y, pArr.x, pArr.y, list);
	if (chemin == NULL)
	{
		*status = -1;
		return 0;
	}
	p = chemin;
	while (p !=NULL)
	{
		pos.x = p->x;
		pos.y = p->y;
		PRINTF("i=%d\r",i);
		i+=genererActionsToXYPos(tab+i, pos);
		PRINTF("A* coor %d,%d  i=%d\r",pos.x,pos.y,i);
		p = p->queue;
	}
	*status = 0;
	return i;
}

int pointDansDisque(Pos p, Pos centre, int rayon)
{
	if ((p2(p.x-centre.x)+p2(p.y-centre.y))<p2(rayon))
		return 1;
	return 0;
}


/* [] END OF FILE */

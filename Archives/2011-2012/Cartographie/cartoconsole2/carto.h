#ifndef CARTO_H_INCLUDED
#define CARTO_H_INCLUDED


//#define DEUX_ROBOT_ADV

#include "astar.h"
#include "malloc.h"
#include "data.h"
#include "message.h"
//#include "uart.h"





void init();
void init_osc();
//void initToDo();
void carto(Point list, Coor chemin);


#endif // CARTO_H_INCLUDED

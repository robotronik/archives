#include <p33FJ128MC802.h>

#include "astar.h"
#include "malloc.h"
#include "data.h"

const unsigned char heap[HEAP_SIZE];
unsigned char* heap_ptr;
unsigned char heap2[HEAP_SIZE2];
unsigned char* heap_ptr2;

int main()
{
    int i,j;
    TRISBbits.TRISB12 = 0;//output
    LATBbits.LATB12 = 0;
    TRISBbits.TRISB3 = 0;//output
    LATBbits.LATB3 = 0;//valeur de la sortie

    Point list = NULL;
    Coor chemin = NULL;
    int startId = 1, goalId = 318;

    my_malloc_init();


    list = loadPointList();

    chemin = aStar(startId,goalId,list);


    int x = list->x;
    int y = list->y;
    int id = list->id;
    int f = list->lPt->id;

    int x2 = list->queue->x;
    int y2 = list->queue->y;
    int id2 = list->queue->id;
    int f2 = list->queue->lPt->id;

    int xc = chemin->x;
    int yc = chemin->y;
    int idc = chemin->id;

    int xc2 = chemin->queue->x;
    int yc2 = chemin->queue->y;
    int idc2 = chemin->queue->id;

    my_free(NULL);//libere tout le tas 2 (heap2)

    /*int size_set = sizeof(struct Set);
    int size_data = sizeof(struct Data);
    int size_came = sizeof(struct Came);
    int size_coor = sizeof(struct Coor);*/


    while (1)
    {
       LATBbits.LATB3 = ~LATBbits.LATB3;

       for ( i = 0 ; i < 30000 ; i++)
       {
            for ( j = 0 ; j < 5 ; j++)
            {

            }
       }
    }
    return 0;
}





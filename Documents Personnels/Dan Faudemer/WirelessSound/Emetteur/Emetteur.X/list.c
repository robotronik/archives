#include <pic18f4550.h>
#include "list.h"

char empty_list(list *myList)
{
    return myList->start == myList->end;
}

char add_list(list *myList, char val)
{
    if(((myList->end+1)%LIST_SIZE) == myList->start) /* Fifo is full*/
        return -1;

    /* Add value in the fifo*/
    myList->val[myList->end] = val;
    /* Move end of the fifo*/
    myList->end = (myList->end+1)%LIST_SIZE;

    return 1;

}

char pop_list(list *myList, char *val)
{
    if(myList->start == myList->end)/* Fifo is empty*/
    {
        return -1;
    }

    *val = myList->val[myList->start];
    myList->start = (myList->start+1) %LIST_SIZE;

    return 1;

}

void init_list(list *myList)
{
    myList->start = 0;
    myList->end = 0;
    return ;
}
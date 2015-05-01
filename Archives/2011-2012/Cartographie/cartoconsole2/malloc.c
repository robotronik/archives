#include "malloc.h"

void my_malloc_init(void)
{
    heap_ptr = heap;
    heap_ptr2 = heap2;
}

void *my_malloc(unsigned long len)
{
    unsigned char *p = heap_ptr;
    heap_ptr += len;
    if (heap_ptr >= heap + HEAP_SIZE)
    {
        #ifndef SIMU
        #else
        printf("Erreur d'allocation dans la carto\n");
        #endif
        return NULL;
    }
    else
        return p;
}


void my_free(void* p)
{
    // do nothing for heap
	heap_ptr2 = heap2; // heap2 est remis à zero
}

void *my_malloc2(unsigned long len)
{
    unsigned char *p2 = heap_ptr2;
    heap_ptr2 += len;
    if (heap_ptr2 >= heap2 + HEAP_SIZE2)
    {
        #ifndef SIMU
        #else
        printf("Erreur d'allocation dans la carto\n");
        #endif
        return NULL;
    }
    else
        return p2;
}


#include "malloc.h"
#include "constantes.h"


//const unsigned char heap[HEAP_SIZE];
//unsigned char heap[HEAP_SIZE];
//unsigned char* heap_ptr;
//const unsigned char heap2[HEAP_SIZE2];
unsigned char heap2[HEAP_SIZE2];
unsigned char* heap_ptr2;

int erreurAlloc = 0;

void my_malloc_init(void)
{
    //heap_ptr = heap;
    heap_ptr2 = heap2;
}

/*void *my_malloc(unsigned long len)
{
    unsigned char *p = heap_ptr;
    heap_ptr += len;
    if (heap_ptr >= heap + HEAP_SIZE)
    {
	    //Ne doit jamais arrivé ici, la taille du tas est prévu pour
        PRINTF("Erreur d'allocation dans la carto\r");
        return NULL;
    }
    else
        return p;
}*/


void my_free(void* p)
{
    // do nothing for heap
	heap_ptr2 = heap2; // heap2 est remis à zero
	erreurAlloc = 0;
}

void *my_malloc2(unsigned long len)
{
    unsigned char *p2 = heap_ptr2;
    heap_ptr2 += len;
    if (heap_ptr2 >= heap2 + HEAP_SIZE2)
    {

		//PRINTF("#################################\r");
        PRINTF("Erreur d'allocation dans la carto\r");
		//PRINTF("#################################\r");
		erreurAlloc = 1;
		heap_ptr2 = heap2;
        return heap_ptr2; //NULL;
    }
    else
        return p2;
}

int testAlloc(void)
{
	if (heap_ptr2 >= heap2 + HEAP_SIZE2) 
    {
		PRINTF("Erreur de malloc, on s'arrete\r");
		return -1;
	}
	if (erreurAlloc)
	{
		PRINTF("Erreur de malloc, on s'arrete 2\r");
		return -1;
	}
	return 0;
}


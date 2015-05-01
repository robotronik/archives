#ifndef MALLOC_H_INCLUDED
#define MALLOC_H_INCLUDED

#include <p33FJ128MC802.h>

/*Le tas heap est remplit par les fonctions addPoint() et addListeFils(),
grâce à my_malloc(), il ne peut être libéré, my_free() est inutile.
Le tas 2 heap2 est remplit par addSet(), addData(), addCame() et addCoor(),
grâce à my_malloc2(), il est libéré par my_free() qui efface 
TOUTES les allocations, càd toutes les listes Set, Data, ...*/

#define NULL 0
#define HEAP_SIZE 12500//3210
#define HEAP_SIZE2 3000//2000

extern const unsigned char heap[HEAP_SIZE];
extern unsigned char* heap_ptr;
extern unsigned char heap2[HEAP_SIZE2];
extern unsigned char* heap_ptr2;

void my_malloc_init(void);
void *my_malloc(unsigned long len);
void my_free(void* p);
void *my_malloc2(unsigned long len);

#endif // MALLOC_H_INCLUDED



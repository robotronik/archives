#ifndef MALLOC_H_INCLUDED
#define MALLOC_H_INCLUDED

/*
Le tas heap est remplit par les fonctions addPoint() et addListeFils(),
grâce à my_malloc(), il ne peut être libéré, my_free() est inutile.
Le tas 2 heap2 est remplit par addSet(), addData(), addCame() et addCoor(),
grâce à my_malloc2(), il est libéré par my_free() qui "efface"
TOUTES les allocations, càd toutes les listes Set, Data, ...
*/

//#define NULL 0
//#define HEAP_SIZE 20000//2900//1000//11300 //consommation : 11228
#define HEAP_SIZE2 25000//4000



void my_malloc_init(void);
//void *my_malloc(unsigned long len);
void my_free();
void *my_malloc2(unsigned long len);
int testAlloc(void);

#endif // MALLOC_H_INCLUDED



#ifndef COMASSER_H
#define COMASSER_H

#define TAILLE_FILE_ASSER 8

struct _InFileAsser
{
	short x,y;
};
typedef struct _InFileAsser InFileAsser;

struct _FileAsser
{
	InFileAsser v[TAILLE_FILE_ASSER];
	char debut, fin;
};
typedef struct _FileAsser FileAsser;

void init_comAsser();

#endif
#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <p33FJ128MC802.h>

struct ACalculer
{
	unsigned char nbUsRecues;  // Nombre de trames ultrasons recues
	unsigned char echec; // Le ToDo courant a ete un echec
	unsigned short time[3];
};
typedef struct ACalculer ACalculer;

extern ACalculer toDo;	
		

void init_osc();
void initToDo();


#endif // INIT_H_INCLUDED



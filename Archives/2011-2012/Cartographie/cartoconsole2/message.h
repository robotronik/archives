#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include "uart.h"

struct _aTraiter
{
	char chemin:3,
		commencer:2,
		complet:2;//boolean
	char numero;
    int xNousDep;
    int yNousDep;
    int xNousArr;
    int yNousArr;

	int xAdv;
    int yAdv;
	#ifdef DEUX_ROBOT_ADV
    int xAdv2;
    int yAdv2;
	#endif
  
};

typedef struct _aTraiter aTraiter;

extern aTraiter toDo;
extern aTraiter toToDo;

void message_processing(unsigned char inMessage);
void initToDo();
void initToToDo();



#endif // MESSAGE_H_INCLUDED


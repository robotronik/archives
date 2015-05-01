#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include "uart.h"

/*message_processing() re�oit des trames de l'uart et les met dans toToDo
quand c'est complet elle copie totoDo dans toDo pour qu'il soit calcul�
= une genre de file de 2*/

struct _ATraiter
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

typedef struct _ATraiter ATraiter;

extern ATraiter toDo;
extern ATraiter toToDo;

void message_processing_carto(unsigned char inMessage);
void initToDo();
void initToToDo();



#endif // MESSAGE_H_INCLUDED


#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include "uart.h"

/*message_processing() reçoit des trames de l'uart et les met dans toToDo
quand c'est complet elle copie totoDo dans toDo pour qu'il soit calculé
= une genre de file de 2*/

/*****TRAME UART********/
/*carto => pSoc :

trame du "début" :
|1011|nombre|num(id)|   longueur   |
   4    6       6        16        

trame de données (répété autant de fois que nombre) :
|010|num(id)|     X      |      Y     |
  3     6         12           11      

pSoc => carto :

|R|D|L|num(id)|      X      |     Y     |
 1 1 1   6           12          11      

L : choix chemin +longueur ou longueur uniquement,
1: longueur +chemin,
0: longueur uniquement

R: choix notre robot/robot adverse,
1: notre robot,
0: robot adverse

D:
1: Départ (ou robot adverse 1)
0: Arrivé (ou robot adverse 2)
*/

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


#include "uart.h"


void sendCoor(Coor list, int longueur, int longPlusChemin)
{
	static unsigned char numero = 0 ;
	numero++;
    Coor p = list;
    trame tr;
    trame tr2;
	tr.l =0;
	tr2.l =0;


	/*

	if ( !longPlusChemin )//càd uniquement longueur
	{
		tr.first.longueur = longueur;
		tr.first.numero = numero;
		tr.first.nb = 0;
		tr.first.code = 0b1011;
		send_message(tr.l);
		return;
	}*/

	tr.first.longueur = longueur;
	tr.first.numero = numero;
	tr.first.nb = nbCoor(list);
	tr.first.code = 0b1011;
	printf("%x %i\n",tr.l,tr.first.longueur);

	while ( p != NULL)
    {
	    tr2.l = 0;
		tr2.data.y = p->y;
		tr2.data.x = p->x;
		tr2.data.numero = numero;
		tr2.data.code = 0b010;
		printf("%x %i %i\n",tr2.l,tr2.data.x,tr2.data.y);
        p = p->queue;
    }


}


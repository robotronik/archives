#include "message.h"

ATraiter toDo;
ATraiter toToDo;


void initToDo()
{
	toDo.commencer = 0;
	toDo.complet = 0;
	toDo.chemin = 0b10;
	toDo.numero = -1;
	toDo.xNousDep = -1;
	toDo.yNousDep = -1;
	toDo.xNousArr = -1;
	toDo.yNousArr = -1;
	toDo.xAdv = -1;
	toDo.yAdv = -1;
}

void initToToDo()
{
	toToDo.commencer = 0;
	toToDo.complet = 0;
	toToDo.chemin = 0b10;
	toToDo.numero = -1;
	toToDo.xNousDep = -1;
	toToDo.yNousDep = -1;
	toToDo.xNousArr = -1;
	toToDo.yNousArr = -1;
	toToDo.xAdv = -1;
	toToDo.yAdv = -1;
}

void message_processing_carto(unsigned char inMessage)//modifie toToDo et le copie dans toDo quand c'est complet
{
	static char compteur =1;
	static Trame tr;

	if (compteur == 1)
		{
			tr.charx4.Xooo = inMessage;
			compteur++;
			return;
		}
	else if (compteur == 2)
		{
			tr.charx4.oXoo = inMessage;
			compteur++;
			return;
		}
	else if (compteur == 3)
		{
			tr.charx4.ooXo = inMessage;
			compteur++;
			return;
		}
	else if (compteur == 4)
		{
			tr.charx4.oooX = inMessage;
			compteur = 1;
			//c'est complet
		}
	else
		{
			compteur = 1;
			return;
		}
	//ici la trame est complete
	if ( toToDo.commencer == 0b01 && toToDo.numero != tr.recep.numero )
	{
		toToDo.commencer = 0b00;
		initToToDo();
		//les données d'avant n'est pas complete on on recommence avec une nouvelle
	}
	if ( toToDo.commencer == 0b00) //nouvelle reception
	{
		toToDo.numero = tr.recep.numero;
		toToDo.commencer = 0b01;
		toToDo.chemin = 0b000;
		toToDo.chemin = tr.recep.l;
		switch (tr.recep.r)
		{
			case 0b1://notre robot
			{
				switch (tr.recep.d)
				{
					case 0b1:
					{
						toToDo.xNousDep = 0;
						toToDo.yNousDep = 0;
						toToDo.xNousDep = tr.recep.x;
						toToDo.yNousDep = tr.recep.y;
						break;
					}
					case 0b0:
					{
						toToDo.xNousArr = 0;
						toToDo.yNousArr = 0;
						toToDo.xNousArr = tr.recep.x;
						toToDo.yNousArr = tr.recep.y;
						break;
					}
				}
				break;
			}
			case 0b0://robot Adv
			{
				toToDo.xAdv = 0;
				toToDo.yAdv = 0;
				toToDo.xAdv = tr.recep.x;
				toToDo.yAdv = tr.recep.y;
				break;
			}
		}
		return;
	}
	else if ( toToDo.commencer == 0b01 && toToDo.numero == tr.recep.numero )
	{
		switch (tr.recep.r)
		{
			case 0b1://notre robot
			{
				switch (tr.recep.d)
				{
					case 0b1:
					{
						toToDo.xNousDep = 0;
						toToDo.yNousDep = 0;
						toToDo.xNousDep = tr.recep.x;
						toToDo.yNousDep = tr.recep.y;
						break;
					}
					case 0b0:
					{
						toToDo.xNousArr = 0;
						toToDo.yNousArr = 0;
						toToDo.xNousArr = tr.recep.x;
						toToDo.yNousArr = tr.recep.y;
						break;
					}
				}
				break;
			}
			case 0b0://robot Adv
			{
				toToDo.xAdv = 0;
				toToDo.yAdv = 0;
				toToDo.xAdv = tr.recep.x;
				toToDo.yAdv = tr.recep.y;
				break;
			}
		}
		
	}
	else if ( toToDo.complet == 0b01 )
	{
			// normalement pas trop possible
	}

	if (toToDo.xNousDep != -1 && toToDo.xNousArr != -1 && toToDo.xAdv != -1)
	{	
		//LATBbits.LATB5 = 0;//debug LED	
		toToDo.complet = 0b01;
		toDo = toToDo;
		initToToDo();
		return;
	}
	return;
			
}

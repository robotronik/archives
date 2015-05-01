#ifndef MACHINE_H
#define MACHINE_H

enum _State
{
	ASK_PARCOURS, COMMANDE_ASSER, IDLE
};
typedef enum _State State;

void doEtat();

#endif

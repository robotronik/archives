#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#define ADRESSE 0b100

extern volatile char n;
extern volatile char salve;
extern volatile char calibre;
extern unsigned short x,y;
extern volatile char tnAsked;
extern volatile unsigned short tToSend;
extern volatile char posAsked;

#include "init.h"

void message_processing(unsigned char inMessage);

#endif // MESSAGE_H_INCLUDED


#include "uart.h"

using namespace Simu;

MessageUart::MessageUart(unsigned char _message, float _t) : suivant(NULL), t(_t), message(_message)
{
}

MessageUart::~MessageUart()
{
}

FileMessageUart::FileMessageUart() : dernier(NULL), taille(0)
{
}

FileMessageUart::~FileMessageUart()
{
    MessageUart* buff;
    while(!isEmpty())
    {
        buff = pop();
        delete buff;
    }
}

void FileMessageUart::push(MessageUart* message)
{
    if(dernier == NULL)
    {
        message->suivant = message;
        dernier = message;
    }
    else
    {
        message->suivant = dernier->suivant;
        dernier->suivant = message;
        dernier = message;
    }
    taille++;
}

MessageUart* FileMessageUart::pop()
{
    if(isEmpty())
        return NULL;
    MessageUart* retour = dernier->suivant;
    if(dernier->suivant == dernier) // Singleton
        dernier = NULL;
    else
        dernier->suivant = retour->suivant;
    taille--;
    return retour;
}

void FileMessageUart::afficher()
{
    if(isEmpty())
    {
        printf("[vide]\n");
        return;
    }
    MessageUart* buff = dernier->suivant;
    do
    {
        printf("%c ", buff->message);
        buff = buff->suivant;
    }
    while(buff != dernier->suivant);
    printf("\n");
}

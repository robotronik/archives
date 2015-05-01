#ifndef UART_SIMU_H_INCLUDED
#define UART_SIMU_H_INCLUDED

#include <cstdio>

namespace Simu
{
class MessageUart
{
    public :
        MessageUart(unsigned char _message, float _t = 0);
        ~MessageUart();

        MessageUart* suivant;
        float t;
        unsigned char message;
};

class FileMessageUart
{
    public :
        FileMessageUart();
        ~FileMessageUart();

        void push(MessageUart* message);
        MessageUart* pop();
        bool isEmpty() {return dernier == NULL;}
        void afficher();

        void falseEmpty() {dernier = NULL;}
    private :
        MessageUart* dernier;
        unsigned int taille;
};

}


#endif // UART_H_INCLUDED

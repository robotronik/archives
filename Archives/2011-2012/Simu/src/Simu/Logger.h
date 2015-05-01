#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

/*
    Classe singleton au meme niveau que Data.h
    Sert a l 'affichage du mini panneau
*/

namespace Simu
{

class Logger
{
    public:
        Logger();
        virtual ~Logger();

        static void setLog(int place, wchar_t texte[]);  // Exemple :   GraphicEngine::setLog(21, L"Perdu");  swprintf
        static void setLog(int place, char texte[]);  // Exemple :   GraphicEngine::setLog(21, "Perdu");

        static wchar_t* getTextLog(int index) {return (wchar_t*)(cetteInstance->textLog[index]);}
        static wchar_t textLog[26][71];
    private:
        //26 lignes, commencer a 1
        static Logger* cetteInstance;
};


}

void setLog(int place, wchar_t texte[]);
void setLog(int place, char texte[]);

#endif // LOGGER_H

#include "Logger.h"

using namespace Simu;

Logger* Logger::cetteInstance = NULL;
wchar_t Logger::textLog[26][71];

Logger::Logger()
{
    cetteInstance = this;
    //textLog[26][71] = {{0}};
    wcscpy(textLog[0],L"Logger");
    for(int i=1; i<26;i++)
        wcscpy(textLog[i],L" ");
}

Logger::~Logger()
{
    //dtor
}

void Logger::setLog(int place, wchar_t texte[])
{
    wchar_t trueTexte[71];

    wcsncpy(trueTexte, texte, 70);
    trueTexte[70] = 0; // Au cas ou
    for(int i=0; i<71; i++)
    {
        if(trueTexte[i]=='\n')
        {
            trueTexte[i] = 0;
            break;
        }
    }
    wcscpy(cetteInstance->textLog[place], trueTexte);
}

void Logger::setLog(int place, char texte[])
{
    wchar_t trueTexte[71];

    swprintf(trueTexte, L"%s", texte);
    trueTexte[70] = 0; // Au cas ou
    for(int i=0; i<71; i++)
    {
        if(trueTexte[i]=='\n')
        {
            trueTexte[i] = 0;
            break;
        }
    }
    wcscpy(cetteInstance->textLog[place], trueTexte);
}

void setLog(int place, wchar_t texte[])
{
    Logger::setLog(place, texte);
}

void setLog(int place, char texte[])
{
    Logger::setLog(place, texte);
}


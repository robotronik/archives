#ifndef IENGINE_H
#define IENGINE_H

#include "Data.h"
#include "messages.h"
#include <queue>


#define FREQ_GAME 70.0f
#define FREQ_JOYS 35.0f
#define FREQ_PHYS 250.0f // un grand nombre ameliore la stabilite
#define FREQ_IRRL 70.0f
#define FREQ_CMPT 10.0f // Pas besoin d une grande frequence
#define FREQ_IA   70.0f

namespace Simu
{


struct Message
{
    int type;
    void* data;
};

struct Proc
{
    void* engine;
    float dt;
    float lastAppel; // (s) Plus il est important, plus il y aura de chance que ce soit lui qui soit execute. Si negatif, il ne sera pas execute
    float retard;
    float periode;// (s)
    int nbAppels;
};

struct Adresse
{
    void* gameEngine;
    void* physicEngine;
    void* graphicEngine;
    void* joysEngine;
    void* cmptEngine;
};

class IEngine
{
    public:
        IEngine(int freq = 0) { if(freq != 0) {creerProc(this, freq);}  }
        virtual ~IEngine() {}

        virtual void run(float dt) = 0;  // dt en secondes

        inline void sendMessage(void* dest, int type, void* data, bool makeUnique)
        {
            Message m;
                m.type = type;
                m.data = data;
            bool add = true;
            if(makeUnique)
            {
                // On regarde si il existe dans la destination
                std::deque<Message>::iterator it = messages.begin();
                for ( ;it!=messages.end(); it++)
                {
                    if ( it->type == type )
                    {
                        add = false;
                        break;
                    }
                }
            }

            if(add)
                ((IEngine*)dest)->messages.push_front(m);
        }

    protected :

        std::deque<Message> messages;
        static DataSimu* data;
        static std::vector<Proc> processus;
        static Adresse adresse;

    private :

        inline void creerProc(IEngine* engine, int freq)
        {
            Proc proc;

            proc.engine = engine;
            proc.periode = 1.0f/freq;
            proc.dt = 0.0f;
            proc.lastAppel = 0.0f;
            proc.retard = -1.0f;
            proc.nbAppels = 0;

            processus.push_back(proc);
        }
};


}

#endif // ENGINE_H

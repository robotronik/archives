#include "debug.h"
#include <p33Fxxxx.h>

inline void debugLed1(Of o)
{
    DEBUG_LED_1 = o;
}

inline void debugLed1Toggle(void)
{
    DEBUG_LED_1 = ~DEBUG_LED_1;
}

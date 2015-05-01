#include <pic18f4550.h>
#include "list.h"
#include "init.h"
extern list ADCValue;
void interrupt interrupt_function()
{
    int value;
    if(PIR1bits.ADIF == 1 && PIE1bits.ADIE==1)
    {
        PIR1bits.ADIF = 0; /* Clear interrupt flag */
        value = (ADRESH << 8) | ADRESL;
        if(!add_list(&ADCValue, value >> 5)) //List is full => Light a led
        {
            LED_FULL = 1;
        }
        return ;
        
    }
    else if(PIR1bits.TMR2IF == 1 && PIE1bits.TMR1IE==1)
    {
        PIR1bits.TMR2IF = 0; //Clear the flag
        //Start an ADC conversion
        ADCON0bits.GO = 1;
        return ;
    }


}

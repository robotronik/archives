#include <p33EP512MU810.h>
#include "interrupt.h"
#include "adc.h"


extern volatile int tableau_plein;

//ADC1 conversion finished
/*void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void)
{
    tab_adc_brut[adc2receiver(cur_receiver)][cur_sample] = ADC1BUF0;
    IFS0bits.AD1IF = 0;		// Clear the ADC1 Interrupt Flag

    cur_receiver++;
    if(cur_receiver >= NB_RECEIVER)
    {
        cur_receiver = 0;
        cur_sample ++;
    }

}
*/

void __attribute__((interrupt, auto_psv)) _DMA0Interrupt(void)
{
    tableau_plein = 1;
    IFS0bits.DMA0IF = 0; //Clear the DMA0 Interrupt Flag
}
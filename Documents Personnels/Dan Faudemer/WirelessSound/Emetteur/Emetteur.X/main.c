#include <pic18f4550.h>
#include "init.h"
#include "rfm70.h"
#include "list.h"
list ADCValue;

void main() {
    char value;
    init_clock();
    init_led();
    init_ADC();
    init_spi();
    init_list(&ADCValue);
    rfm70_init();


    rfm70_mode_transmit();
    while(1)
    {
        if(!pop_list(&ADCValue, &value))
        {
            /*Ignore the acknowlage messafe*/
            rfm70_transmit_message_once(&value,1);

        }
        
    }
    
}
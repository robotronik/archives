/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "sharp.h"
#include <device.h>

#define NIVEAU_SHARP 2000

volatile int presenceSharp;

void verifierSharp()
{
    int niveau;
    niveau = ADCSharp_GetResult16();
    if(niveau>NIVEAU_SHARP)
    {
        presenceSharp = 1;
    }
    else
    {
        presenceSharp = 0;
    }  
}
/* [] END OF FILE */

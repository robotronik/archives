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

#define NIVEAU_SHARP 1000

volatile int presenceSharp;
//static int sharpEnCours = 0;

void verifierSharp()
{
    int niveau1,niveau2;
    niveau1 = ADC_Sharp_1_GetResult16();
    niveau2 = ADC_Sharp_2_GetResult16();
    //if(Comp_Sharp_1_GetCompare() || Comp_Sharp_2_GetCompare())
    if(niveau1>NIVEAU_SHARP || niveau2 > NIVEAU_SHARP)
    {
        presenceSharp = 1;
    }
    else
    {
        presenceSharp = 0;
    }  
    //sharpEnCours = 1-sharpEnCours;
    //MuxSharp_Select(sharpEnCours);
    
}
/* [] END OF FILE */

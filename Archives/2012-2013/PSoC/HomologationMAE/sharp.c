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
#include <math.h>
#include "ADCSharp_AMux.h"
#include "MuxSharp.h"
#include "ADCSharp.h"

extern volatile int stopSharp;
extern volatile int posX;
extern volatile int posY;
extern volatile int alpha;

void checkPos()
{
    int analog; 
	double x,y;
	
	/*if(Numero_Sharp==1)
	{
		MuxSharp_Select(0);
	}
	else if(Numero_Sharp==2)
	{
		MuxSharp_Select(1);
	}*/
	
	
	MuxSharp_Select(1);
	
	
	ADCSharp_StartConvert();
    analog=ADCSharp_GetResult16();
	/*if(analog>SEUIL15)
    {
        x=posX+cos(alpha)*150;
        y=posY+cos(alpha)*150;
        if(x<3000 && x>0 && y<2000 && y>0 && ((x-1500)*(x-1500)+(y-2000)*(y-2000))<1000000) stopSharp=1;
        else stopSharp=0;
     }
	else*/ if(analog>SEUIL30)
    {
    	/*x=posX + cos(alpha)*150;
        y=posY + cos(alpha)*150;
        if(x<3000 && x>0 && y<2000 && y>0 && ((x-1500)*(x-1500)+(y-2000)*(y-2000))<1000000) stopSharp=1;
      	else stopSharp=0;*/
		stopSharp = 1;
    }
    else stopSharp=0;
	
}
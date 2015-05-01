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
#include "constantes.h"

#define DEG_TO_RAD (3.14159265359/180.0)

int stopSharp;
Pos posObstacle;
/*extern volatile int posX;
extern volatile int posY;
extern volatile int alpha;*/
extern volatile Pos robotNous;

static volatile int detectionEnabled = 1;

void checkPos()
{
    int analog; 
	double x,y;
	static int numSharp = 0;
	numSharp++;
	numSharp %= 2;
	
	//ADCSharp_StartConvert();
    analog=ADCSharp_GetResult16();
	//DEBUG_PRINTF("adc= %d",analog);
	/*if(analog>SEUIL15)
    {
        x=posX+cos(alpha)*150;
        y=posY+cos(alpha)*150;
        if(x<3000 && x>0 && y<2000 && y>0 && ((x-1500)*(x-1500)+(y-2000)*(y-2000))<250000) stopSharp=1;
        else stopSharp=0;
     }
	else*/ 
	//*
	if (detectionEnabled)
	{
		if(analog>SEUIL30)
	    {
	    	x=robotNous.x + cos(robotNous.alpha*DEG_TO_RAD)*300;
	        y=robotNous.y + sin(robotNous.alpha*DEG_TO_RAD)*300;
	        if(x<2950 && x>50 && y<1550 && y>50 && ((x-1500)*(x-1500)+(y-2000)*(y-2000))>250000) stopSharp=5;
	      	else stopSharp--;
			//stopSharp = 1;
			posObstacle.x = (int)x;
			posObstacle.y = (int)y;
			PRINTF("ObtVu %d,%d,%d,%d",numSharp,analog,(int)x,(int)y);
	    }
	    else 
		{
			stopSharp--;
		}
		if (stopSharp<0)
			stopSharp = 0;
	}
	else
	{
		stopSharp = 0;
	}
	MuxSharp_Select(numSharp);
	/*/
	stopSharp = 0;
	//*/
	
}

void enableDetection(void)
{
	detectionEnabled = 1;
}

void disableDetection(void)
{
	detectionEnabled = 0;
}
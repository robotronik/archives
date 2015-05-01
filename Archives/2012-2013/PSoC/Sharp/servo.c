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

#include "servo.h"

static unsigned int position = 20;
static unsigned int sens = 1;


int detectionSharp(int Numero_Sharp) //fonction principale de détection sharp => sharp 1 = avant, sharp 2 =arrière
{
    int analog; double x,y
	
		if(Numero_Sharp==1)
		{
			MuxSharp_Select(0);
		}
		else if(Numero_Sharp==2)
			{
				MuxSharp_Select(1);
			}
			else return(1);
	
	ADCSharp_StartConvert();
    analog=ADCSharp_GetResult16();
	if(analog>SEUIL15)
    {
        x=x_robot+cos(alpha_robot)*150;
        y=y_robot+cos(alpha_robot)*150;
        if(x<3000 && x>0 && y<2000 && y>0 && ((x-1500)*(x-1500)+(y-2000)*(y-2000))<1000000) stop=1;
        else stop=0;
     }
     else if(analog>SEUIL30)
        {
            x=x_robot+cos(alpha_robot)*150;
            y=y_robot+cos(alpha_robot)*150;
            if(x<3000 && x>0 && y<2000 && y>0 && ((x-1500)*(x-1500)+(y-2000)*(y-2000))<1000000) stop=1;
            else stop=0;
        }
        else stop=0;
     else return(0);
	
	

}
		
		

void moveServo(void) {
	if(sens) {
		position++;
		if(position == 70)
			sens = 0;
	} else {
		position--;
		if(position == 20)
			sens = 1;
	}
	PWMServoSharp_WriteCompare(position);
}

void afficheLED(unsigned int result)
{
	if (result < 1400) //affichage des LEDs, pour tester le programme
		{
			LED1_Write(0); 
			if (result < 1200)
			{
				LED2_Write(0);
				if (result < 1000)
				{
					LED3_Write(0);
					if (result < 800)
					{
						LED4_Write(0);
						if (result < 600)
						{
							LED5_Write(0);
							if (result < 400)
							{
								LED6_Write(0);
								if (result < 200)
								{
									LED7_Write(0);
								}
								else LED7_Write(255);
							}
							else
							{
								LED7_Write(255);
								LED6_Write(255);
							}
							
						}
						else
						{
							LED7_Write(255);
							LED6_Write(255);
							LED5_Write(255);
						}
					}
					else
					{
						LED7_Write(255);
						LED6_Write(255);
						LED5_Write(255);
						LED4_Write(255);
					}
				}
				else
				{
					LED7_Write(255);
					LED6_Write(255);
					LED5_Write(255);
					LED4_Write(255);
					LED3_Write(255);
				}
			}
			else
			{
				LED7_Write(255);
				LED6_Write(255);
				LED5_Write(255);
				LED4_Write(255);
				LED3_Write(255);
				LED2_Write(255);
			}
			
		}
		else 
		{
			LED7_Write(255);
			LED6_Write(255);
			LED5_Write(255);
			LED4_Write(255);
			LED3_Write(255);
			LED2_Write(255);
			LED1_Write(255);
		}
}

void initTableau(Calibre *calibre)
{
calibre[0].milli=130;
calibre[1].milli=150;
calibre[2].milli=170;
calibre[3].milli=190;
calibre[4].milli=210;
calibre[5].milli=230;
calibre[6].milli=250;
calibre[7].milli=270;
calibre[8].milli=290;
calibre[9].milli=310;
calibre[10].milli=330;
calibre[11].milli=350;
calibre[12].milli=370;
calibre[13].milli=390;
calibre[14].milli=410;
calibre[15].milli=430;
calibre[16].milli=450;
calibre[17].milli=470;
calibre[18].milli=490;
calibre[19].milli=510;
calibre[20].milli=530;
calibre[21].milli=550;
calibre[22].milli=570;
calibre[23].milli=590;
calibre[24].milli=610;
calibre[25].milli=630;
calibre[26].milli=650;
calibre[27].milli=670;
calibre[28].milli=690;
calibre[29].milli=710;
calibre[30].milli=730;
calibre[31].milli=750;
calibre[32].milli=770;
calibre[33].milli=790;
calibre[34].milli=810;
calibre[35].milli=830;
calibre[36].milli=850;
calibre[37].milli=870;
calibre[38].milli=890;
calibre[39].milli=910;
calibre[40].milli=930;
calibre[41].milli=950;
calibre[42].milli=970;
calibre[43].milli=990;
calibre[44].milli=1010;
calibre[45].milli=1030;
calibre[46].milli=1050;
calibre[47].milli=1070;
calibre[48].milli=1090;
calibre[49].milli=1110;
calibre[50].milli=1130;
calibre[51].milli=1150;
calibre[52].milli=1170;
calibre[53].milli=1190;
calibre[54].milli=1210;
calibre[55].milli=1230;
calibre[56].milli=1250;
calibre[57].milli=1270;
calibre[58].milli=1290;
calibre[59].milli=1310;
calibre[60].milli=1330;
calibre[61].milli=1350;
calibre[62].milli=1370;
calibre[63].milli=1390;
calibre[0].volt=2800;
calibre[1].volt=2760;
calibre[2].volt=2680;
calibre[3].volt=2570;
calibre[4].volt=2450;
calibre[5].volt=2310;
calibre[6].volt=2180;
calibre[7].volt=2040;
calibre[8].volt=1920;
calibre[9].volt=1790;
calibre[10].volt=1680;
calibre[11].volt=1580;
calibre[12].volt=1490;
calibre[13].volt=1420;
calibre[14].volt=1340;
calibre[15].volt=1280;
calibre[16].volt=1220;
calibre[17].volt=1170;
calibre[18].volt=1110;
calibre[19].volt=1060;
calibre[20].volt=1020;
calibre[21].volt=990;
calibre[22].volt=950;
calibre[23].volt=910;
calibre[24].volt=880;
calibre[25].volt=840;
calibre[26].volt=820;
calibre[27].volt=780;
calibre[28].volt=760;
calibre[29].volt=720;
calibre[30].volt=700;
calibre[31].volt=680;
calibre[32].volt=650;
calibre[33].volt=620;
calibre[34].volt=600;
calibre[35].volt=580;
calibre[36].volt=570;
calibre[37].volt=550;
calibre[38].volt=530;
calibre[39].volt=500;
calibre[40].volt=480;
calibre[41].volt=470;
calibre[42].volt=450;
calibre[43].volt=430;
calibre[44].volt=410;
calibre[45].volt=390;
calibre[46].volt=370;
calibre[47].volt=360;
calibre[48].volt=330;
calibre[49].volt=320;
calibre[50].volt=300;
calibre[51].volt=280;
calibre[52].volt=250;
calibre[53].volt=240;
calibre[54].volt=210;
calibre[55].volt=190;
calibre[56].volt=170;
calibre[57].volt=150;
calibre[58].volt=130;
calibre[59].volt=100;
calibre[60].volt=80;
calibre[61].volt=60;
calibre[62].volt=50;
calibre[63].volt=40;
}

int conversion(int entree)
{
	int voltage,i,distance; Calibre calibre[64];
	initTableau(calibre);
	voltage=entree*6144/(65536);//entrée du CAN : 0V-6.144V, précision 65536 bits
	i=0;
	distance=0;
	do //parcours du tableau de calibre
	{
		if(calibre[i].volt<voltage)
		{
			if (i!=0) distance=calibre[i-1].milli;
			else distance=calibre[0].milli;
		}
	  i++;
	}while(distance==0 && i<63);
	if (distance==0) distance=30000;
	return(distance);
}
/* [] END OF FILE */

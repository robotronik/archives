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
#include "global.h"
#include "servo.h"
#include "fin.h"
#include "comAsser.h"
#include "comXBee.h"

extern Couleur g_couleur;

void fin()
{
    while(1)
    {
        sendStop();
        printXBee("FIN",3);
    }
}

void lancer_filet()
{
	if( g_couleur == JAUNE )
	{
		lancerGauche( 2 );
	}
	else
	{
		lancerDroit( 2 );
	}
}

/* [] END OF FILE */

/* 
 * File:   main_test_sharp.c
 * Author: medjo
 *
 * Created on 24 mai 2014, 16:08
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "sharp.h"


/*
 * 
 */
volatile char forceIgnoreSharp=1;//au début, premier cadeau

int main(int argc, char** argv)
{
    initSharp() ;
    while(1)
    {
        if(getDistance(SHARP_1) == DISTANCE_PROXIMITE || getDistance(SHARP_1) == DISTANCE_ARRET_URGENCE)
        {
            affiche_debug(DEBUG_0) ;
        }
        else
        {
            affiche_debug(DEL_1) ;
        }
    }

    return (EXIT_SUCCESS);
}


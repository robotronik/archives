#include "fir.h"
#include <p33EP512MU810.h>


void filter_out(int* coef_fir , int * tab_adc_brut , int start_coef , long* res)
{
    /*register int cpt asm("0");
    register int result asm("A"); //res est dans l'accu A
    //register int B asm("B");
    register int * xMem asm("8");
    register int * yMem asm("10");
    register int xVal asm("4");
    register int yVal asm("5");*/
    int cpt;
    int * xMem;
    int * yMem;

   

    xMem = tab_adc_brut;
    yMem = coef_fir;
    if( start_coef == 0 )
    {
       
       /* asm("CLR A,[W8]+=2,W4,[W10]+=2,W5");
        asm("machin:");
        asm("CLR W15");
        asm("INC W0,W0");
        asm("MAC W4*W5, A, [W8]+=2, W4, [W10]+=2, W5");
        asm("SUB #300 , W0");
        asm("BRA Z,machin");*/
        * res = 0;
        //result = __builtin_clr_prefetch(&xMem,&xVal,2,&yMem,&yVal,2,0,0); //clear le registre
        for(cpt=0 ; cpt<NB_COEF_FIR; cpt++)
        {
            *res += tab_adc_brut[cpt + start_coef] * coef_fir[cpt];
           
            //result = __builtin_mac(result,xVal,yVal,&xMem,&xVal,2, &yMem,&yVal,2,0,0);

        }
//        *res=result;

    }
    else // au lieu de tout recalculer, on prend le dernier resultat - coeff trop ancien + dernier coef
    {
        *res = *res - tab_adc_brut[start_coef - 1] * coef_fir[0] + tab_adc_brut[start_coef + NB_COEF_FIR - 1] * coef_fir[NB_COEF_FIR - 1];
    }
}

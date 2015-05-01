/* 
 * File:   fir.h
 * Author: julien
 *
 * Created on 28 avril 2013, 13:31
 */

#ifndef FIR_H
#define	FIR_H

#ifdef	__cplusplus
extern "C" {
#endif

  //NB_COEF_FIR + NB_COEF_PERIODE = 128 (limite DMA)

#define NB_COEF_FIR 85
#define NB_COEF_PERIODE 43  //une période tient sur 31.25 sample

    void filter_out(int * coef_fir , int * tab_adc_brut , int start_coef, long * res);

    inline extern long asmFIR(int * coef_fir, int * tab_adc_brut , int start_coef, int nb_coef_fir);

#ifdef	__cplusplus
}
#endif

#endif	/* FIR_H */


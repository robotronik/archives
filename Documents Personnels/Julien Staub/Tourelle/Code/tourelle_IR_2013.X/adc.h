/* 
 * File:   adc.h
 * Author: julien
 *
 * Created on 28 avril 2013, 13:34
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NB_RECEIVER 32


    void get_adc(void);
    int diode2chanel(int num_diode);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */


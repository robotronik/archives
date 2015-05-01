/* 
 * File:   init.h
 * Author: julien
 *
 * Created on 28 avril 2013, 13:32
 */

#ifndef INIT_H
#define	INIT_H

#ifdef	__cplusplus
extern "C" {
#endif

    inline void init_adc(void);
    //inline void init_timer3(void);
    inline void init_dma0(void);
    inline void init_osc(void);
    inline void init_port(void);


#ifdef	__cplusplus
}
#endif

#endif	/* INIT_H */


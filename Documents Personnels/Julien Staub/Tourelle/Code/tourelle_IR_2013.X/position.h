/* 
 * File:   position.h
 * Author: julien
 *
 * Created on 28 avril 2013, 14:46
 */

#ifndef POSITION_H
#define	POSITION_H

#ifdef	__cplusplus
extern "C" {
#endif

    int rcpt2angle(unsigned long * amplitude);
    void angle2pos(int a, int b, int c, unsigned int * x , unsigned int * y);


#ifdef	__cplusplus
}
#endif

#endif	/* POSITION_H */


/* 
 * File:   mathHelper.h
 * Author: jeremy
 *
 * Created on 30 décembre 2012, 23:20
 */

#ifndef MATHHELPER_H
#define	MATHHELPER_H

#include <stdint.h>

struct Quaternion
{
    float w;
    float x;
    float y;
    float z;
};
typedef struct Quaternion Quaternion;

struct VectorInt16
{
    int16_t x;
    int16_t y;
    int16_t z;
};
typedef struct VectorInt16 VectorInt16;

struct VectorFloat
{
    float x;
    float y;
    float z;
};
typedef struct VectorFloat VectorFloat;

Quaternion Q_getProduct(Quaternion p, Quaternion q);
Quaternion Q_getConjugate(Quaternion q);
void V16_rotate(VectorInt16 *v, Quaternion *q);



#endif	/* MATHHELPER_H */


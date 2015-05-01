#ifndef CALIBREUR_H
#define CALIBREUR_H

#include "PWM.h"
#include "timer.h"

#define PERIODE_TRAME_CALIB 0x002C // 0.5 ms

extern volatile unsigned char calibrage;
void sendSignalCalibrage(char aCalibrer); // 0b01 pour le 100, 0b10 pour le 101, 0b11 pour les 2

#endif

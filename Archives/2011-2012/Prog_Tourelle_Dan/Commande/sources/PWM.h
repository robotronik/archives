#ifndef PWM_H
#define PWM_H

#include <p33FJ128MC802.h>
#include "uart.h"

#define PERIODE_PWM 92 // 40 kHz

void init_pwm();

void emitUS_0();
void emitUS_1();
void emitUS_2();
void emitCalibrage();
void disableEmit();

#endif // PWM_H defined

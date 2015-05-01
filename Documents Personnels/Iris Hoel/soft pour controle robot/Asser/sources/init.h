/*-----------------------------------------------------*
 * Initialisation registre et variables                *
 *-----------------------------------------------------*/




#ifndef _INIT_H
#define _INIT_H

// externe
void init(void);
// interne
void init_osc();
void init_parametre();
void motors_init (void);
void PWM1_init ();
void QEI_init ();
void io_init(void);
void UART_init(void);
void timer_init(void);


#endif

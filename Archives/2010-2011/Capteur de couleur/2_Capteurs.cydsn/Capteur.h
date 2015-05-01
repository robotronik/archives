/*******************************************************************************
* Copyright 2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
/* ========================================
 *
 * This file is automatically generated by PSoC Creator
 * and should not be edited by hand.
 *
 * This file is necessary for your project to build.
 * Please do not delete it.
 *
 * ========================================
*/

#ifndef H_CAPTEUR
#define H_CAPTEUR

#define TIMER_CLK	    12000000
#define MAX_FREQ		500000
#define MAX_DEMI_PERIOD	3060	// 255*TIMER_CLK/(2*MAX_FREQ)

#define SAT_LEVEL	    1
#define BLACK_LEVEL	    10
#define WHITE_LEVEL	    50

#define RED			0
#define GREEN		1
#define BLUE		2
#define REF			3
#define HUE			0
#define SATURATION	1
#define LUMINANCE	2

typedef union{
	struct{
		unsigned int min :2;	// Poids faible
		unsigned int mid :2;
		unsigned int max :4;
	} part;
	char all;
} T_order;

typedef union {
	uint8 group;
	struct {
		uint8 right :1;			// Poids faible
		uint8 left :7;
	} unit;
} T_sensors;

void init_capteur(void);
void capture_demi_period(int8 num_capteur);
void LED_change(int couleur);
void acquisition(int8 sensor_num, uint8 color[], float coeff[]);
void etalonnage (int8 sensor_num, uint8 color[], float coeff[]);
int8 convert_RGB_to_HSL(uint8 color[]);
uint8 sort_compos(uint8 color[]);

#endif

/*[]*/
/* ========================================
 *
 * Copyright Phelma, 2011
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF PHELMA.
 *
 * ========================================
*/

#include <device.h>

int8 is_error[2] = {1, 1};

void init_capteur(void) {
	Counter_LED_Start();
	isr_LED_Start();
	isr_LED_Disable();
	
	Counter_L_Start();
	Counter_R_Start();
}

void LED_change(int couleur) {	
	LEDR_Write(0);
	LEDG_Write(0);
	LEDB_Write(0);
	switch (couleur) {
	case RED :
		LEDR_Write(1);
		break;
	case GREEN :
		LEDG_Write(1);
		break;
	case BLUE :
		LEDB_Write(1);
	}
}

void acquisition(int8 sensor_num, uint8 color[], float coeff[]) {
	static float reference[2];
	uint16 demi_period;
	
	// Prend une demi-période au hazard dans le FIFO parmis 4 valeurs
	if (sensor_num == LEFT) demi_period = Counter_L_ReadCapture();
	else demi_period = Counter_R_ReadCapture();
	
	if (demi_period <= 857) {	// 71 µs max
		switch (Counter_LED_ReadCounter()) {
		case REF :	// Le compteur vient de se décrémenter
			color[RED] = (MAX_DEMI_PERIOD/(float)demi_period -
				reference[sensor_num]) * coeff[RED];
			break;
		case GREEN - 1 :
			color[GREEN] = (MAX_DEMI_PERIOD/(float)demi_period -
				reference[sensor_num]) * coeff[GREEN];
			break;
		case BLUE - 1 :
			color[BLUE] = (MAX_DEMI_PERIOD/(float)demi_period -
				reference[sensor_num]) * coeff[BLUE];
			break;
		default :
			reference[sensor_num] = MAX_DEMI_PERIOD/(float)demi_period;
		}
	} else is_error[sensor_num] = 1;
}

void etalonnage (int8 sensor_num, uint8 color[], float coeff[]) {
	if (color[RED] == 0 || color[GREEN] == 0 || color[BLUE] == 0) {
		if (sensor_num) CyPins_ClearPin(DebugH_2);
		else CyPins_ClearPin(DebugH_1);
	} else {
		if (sensor_num) CyPins_SetPin(DebugH_2);
		else CyPins_SetPin(DebugH_1);
	}
	if (color[RED] != 0) coeff[RED] = 64 / (color[RED] / coeff[RED]);
	if (color[GREEN] != 0) coeff[GREEN] = 64 / (color[GREEN] / coeff[GREEN]);
	if (color[BLUE] != 0) coeff[BLUE] = 64 / (color[BLUE] / coeff[BLUE]);
}

int8 convert_RGB_to_HSL(uint8 color[])
{
	T_order order;
	unsigned char lum, sat, calcul;

	order.all = sort_compos(color);

	lum = (unsigned char)(( (unsigned short)color[order.part.max] +
		color[order.part.min] ) >> 1);
	if (lum < BLACK_LEVEL) return 1;
	/*if (lum < BLACK_LEVEL || lum > WHITE_LEVEL) return 0;
	
	if (lum > 128) sat = (unsigned char)( (unsigned short)
		(color[order.part.max] - color[order.part.min] ) * 128 / lum);
	else  sat = (unsigned char)( (unsigned short)(color[order.part.max] -
		color[order.part.min] ) * 128 / (256-lum));
	if (sat < SAT_LEVEL) return 0;
	
	calcul = (unsigned char)((unsigned short)((color[order.part.mid] -
		color[order.part.min])<<8) / (color[order.part.max] -
		color[order.part.min]));
	if ((order.all == 0x09 && calcul > 50) ||
		(order.all == 0x21 && calcul > 110)) return 1;	// Violet*/
	return 0;
}
		
	/*if (lum < BLACK_LEVEL);			// Noir
	else if (lum > WHITE_LEVEL);	// Blanc
	else if (sat < SAT_LEVEL);		// Gris
	else
		switch (order.all)
		{
			case 0x06 :
				if (calcul > 30 && calcul <= 170);	// Orange
				else if (calcul <= 30);				// Rouge
				else;								// Jaune
				break;
			case 0x09 :
				if (calcul > 50);					// Violet
				else;								// Rouge
				break;
			case 0x12 :
				if (calcul > 160);					// Vert
				else;								// Jaune
				break;
			case 0x18 :
				if (calcul > 130);					// Bleu
				else;								// Vert
				break;
			case 0x21 :
				if (calcul > 110);					// Violet
				else;								// Bleu
				break;
			case 0x24 :;							// Bleu
		}
}*/

uint8 sort_compos(uint8 color[]) 	// Donne l'ordre des couleurs
{
	if (color[2] > color[1] && color[2] > color[0])
	{
		if (color[1] > color[0])
			return 0x24;	// bleu vert rouge
		return 0x21;		// bleu rouge vert
	}
	if (color[1] > color[0])
	{
		if (color[2] > color[0])
			return 0x18;	// vert bleu rouge
		return 0x12;		// vert rouge bleu
	}
	if (color[2] > color[0])
		return 0x09;	// rouge bleu vert
	return 0x06;		// rouge vert bleu
}

/* [] END OF FILE */

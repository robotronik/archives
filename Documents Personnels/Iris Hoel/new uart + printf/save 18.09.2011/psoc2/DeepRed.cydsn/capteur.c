#include <device.h>

extern int ret;
extern int8 pion;

int pieceAvant(){
	if (!pion) return 1;
	pion = 0;
	return 1;
}
int pieceArriere(){
	return Capteur_pion_ar_Read();
}
int robotDevant(){
	return Capteur_robot_av_g_Read() || Capteur_robot_av_d_Read();
}
int robotArriere(){
	return (0)?1:0;
}





#include "uart.h"
#include "commAsser.h"
#include <iostream>
#include <stdio.h>


//#define CONSOLE
/*=============================================================================
 *
 *
 *                      Control du robot par joystick
 *
 *
 *===========================================================================*/


static bool traitantEvenement();


using namespace std;


int main ( int argc, char** argv )
{
    /* printf dans la console avec la sdl */
  //  freopen("CON", "w", stdout);
   // freopen("CON", "r", stdin);
    //freopen("CON", "w", stderr);
    int typeAsser,// Alpha ou delta
    typeCoeff,// Coefficiant ( P, D, I ) en alpha ou delta
    typeAsser2;// Alpha ou delta 2ème niveau
    XBee xbee;
    double val;

    char var[500];

    FILE *f = NULL;
    AD_k kP_delta;
    AD_k kD_delta;

    AD_k kP_alpha;
    AD_k kD_alpha;

    //AD_k coeffAlpha;
    //AD_k coeffDelta;

    #ifdef CONSOLE
	while(1)
	{
		cout << "1) Alpha \n2)Delta " << endl;
		cin >> typeAsser;

		cout << "1) Alpha \n2)Delta " << endl;
		cin >> typeAsser2;

		cout << "1) Kp \n2) Kd" << endl;
		cin >> typeCoeff;

		cout << "Valeur :" << endl;
		cin >> val;

		if(typeAsser == 1 )
			typeAsser = ALPHA;
		else if(typeAsser == 2)
			typeAsser = DELTA;
		else
		{
			cout << "erreur" << endl;
			continue;
		}


		if(typeAsser2 == 1 )
			typeAsser2 = ALPHA;
		else if(typeAsser2 == 2)
			typeAsser2 = DELTA;
		else
		{
			cout << "erreur" << endl;
			continue;
		}

		if(typeCoeff == 1 )
			typeCoeff = P;
		else if(typeCoeff == 2)
			typeCoeff = D;
		else
		{
			cout << "erreur" << endl;
			continue;
		}

		sendConfig(xbee, typeCoeff, typeAsser, typeAsser2, val);
_delta
_delta




		system("cls");
	}
	#endif

    //Mode fichier

    f = fopen ("config.txt", "r");
    if(f == NULL){
        printf("Fichier introuvable");
        exit(0);

    }

        while (fgetc(f) != '\n');
        while (fgetc(f) != '\n');
        while (fgetc(f) != '\n');

    fscanf(f,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf", &kP_delta.delta,&kD_delta.delta ,&kP_delta.alpha,&kD_delta.alpha,&kP_alpha.delta,&kD_alpha.delta,&kP_alpha.alpha, &kD_alpha.alpha);


  //  fscanf(f,"%s",var);

   // printf("%s \n", var);
    //Envoi de Delta :


    sendConfig(xbee, P, DELTA, DELTA, kP_delta.delta);
    sendConfig(xbee, D, DELTA, DELTA, kD_delta.delta);
    sendConfig(xbee, P, DELTA, ALPHA, kP_delta.alpha);
    sendConfig(xbee, D, DELTA, ALPHA, kD_delta.alpha);

    //Envoi Alpha
    sendConfig(xbee, P, ALPHA, DELTA, kP_alpha.delta);
    sendConfig(xbee, D, ALPHA, DELTA, kD_alpha.delta);
    sendConfig(xbee, P, ALPHA, ALPHA, kP_alpha.alpha);
    sendConfig(xbee, D, ALPHA, ALPHA, kD_alpha.alpha);





    return EXIT_SUCCESS;
}



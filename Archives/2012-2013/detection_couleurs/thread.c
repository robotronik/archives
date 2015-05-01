


/**********************
 * le programme est lancé par /etc/xdg/lxsession/LXDEautostart
 * **********************/

//pour le thread
#include <pthread.h>
//pour l'uart
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

//pour la vision
#include "couleur.h"
#include "uart.h"
#include "map.h"
#define False  0
#define True   1
#define BLEU 0
#define ROUGE 1

/****************************
 * la variable qui est partagée entre le scrutateur
 * et le programme principal
 * *************************/

int tirette_pulled = False;
static int sendUart = False;
int posX = 0;
int posY = 0;
volatile int couleurEquipe = -1;






char M1,M2,M3,M4,M5,M6; //Couleurs des mires


/**************************/
void* thread_shutdown()
{

    while(!tirette_pulled) 
    { 
	system("sleep 1");
    } 
    system("sleep 90");
    system("shutdown -h now");

    system("sleep 100");
    pthread_exit(0);
}
/***********************/
void * thread_recept_uart()
{
    int fd = open("/dev/ttyAMA0",  O_NOCTTY | O_RDWR ); //On ouvre le protocole de communication
    Trame send; //Message à envoyer
    Trame recu;
    Trame4 trame4; //Trame pour la position 
    int nb_recu = 0;




    if(fd == -1) //Impossible d'ouvrir l'uart
    {
        printf("Impossible d'ouvri AMA0, avez vous les droits ?");
        exit(0);
    }
    struct termios options; //Configuration de l'uart
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200); //Baud rate de 155200
    cfsetospeed(&options, B115200); 
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD; 
    options.c_iflag = IGNPAR | ICRNL; //Gestion du codage de l'uart, parity 
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush (fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    system("sleep 1");
    send.d.codet = CODE_COULEUR;
    write(fd, &send,1 );
    printf("%x\n",send.c);
    system("sleep 1");

    send.d.codet = CODE_COULEUR;
    write(fd, &send,1 );
    printf("%x\n",send.c);

    send.d.codet = CODE_COULEUR;
    write(fd, &send,1 );
    printf("%x\n",send.c);

    send.d.codet = CODE_COULEUR;
    write(fd, &send,1 );
    printf("%x\n",send.c);

    while (1)
    {
//        printf("attente de reception\n ");
        int z = read(fd, (void*)&recu.c,1);
        if( z == -1)
	{
	    printf("Une erreur %s \n", strerror(errno));
	    exit(0);
	}
	printf("Recu %x \n", recu.c);
	if(nb_recu == 0)
	{
		
	    //Décodage :
			
	    if(recu.d.codet == CODE_TIRETTE && couleurEquipe != -1 )
	    {
      		printf("Code tirette recu \n");
		tirette_pulled = True;
		  
		while(!sendUart);//On attend que le main nous redonne la main (que vision soit terminé)
		//Chargement des données
		send.mire.M6 = M6;
		send.mire.M5 = M5;
		send.mire.M4 = M4;
		send.mire.M3 = M3;
		send.mire.M2 = M2;
		send.mire.M1 = M1; 
		send.mire.codet = CODE_TIRETTE;
		//Envoie des données 
		write(fd, &send,1 );
		printf("%x\n",send.c);
		write(fd, &send,1 );
		printf("%x\n",send.c);
		write(fd, &send,1 );
		printf("%x\n",send.c);
		write(fd, &send,1 );
		printf("%x\n",send.c);			

		system("sleep 1");

		write(fd, &send,1 );
		printf("%x\n",send.c);
		write(fd, &send,1 );
		printf("%x\n",send.c);
		write(fd, &send,1 );
		printf("%x\n",send.c);
		write(fd, &send,1 );
		printf("%x\n",send.c);			

	    }
	    else if (recu.d.codet == CODE_POS && tirette_pulled == True)
	    {
		nb_recu++;
		//printf("Pos recu");
		trame4.tr4char.Xooo = recu.c;
	    }
	    else if(recu.d.codet == CODE_COULEUR && couleurEquipe == -1)
	    {
		printf("Code couleur recu");
		couleurEquipe = recu.couleur.couleur;
		send.d.codet = CODE_COULEUR;
		write(fd, &send,1 );
		printf("%x\n",send.c);

					
	    }
	}
	else if(nb_recu == 1)
	{
	    nb_recu++;
	    trame4.tr4char.oXoo = recu.c;
	}
	else if(nb_recu == 2)
	{

	    trame4.tr4char.ooXo = recu.c;
	    nb_recu++;
	}
	else if (nb_recu == 3)
	{

	    trame4.tr4char.oooX = recu.c;
	    posX =  trame4.tpos.x;
	    posY = trame4.tpos.y;
	    nb_recu = 0;
	    //	    printf("Toute la pos recu Trame : %X  X:%d Y:%d\n", trame4,posX, posY);
	}

	

   
    }
    pthread_exit(0);
}


int main(void)
{

    void *ret;
    pthread_t tid_recept_uart; //notre thread de scrutation de l'uart
    pthread_t tid_shutdown ;
    //création du thread de lecture
    if(pthread_create(&tid_recept_uart, NULL, thread_recept_uart, NULL))
    {
        printf("erreur lors de la création du thread de l'uart\n");
    }

    if(pthread_create(&tid_shutdown, NULL, thread_shutdown, NULL))
    {
        printf("erreur lors de la création du thread de shutdown\n");
    }

    system("sh /home/pi/Robotronik/robotronik/detection_couleurs/scriptcam.bash");
    //system("sh -c /home/pi/Robotronik/robotronik/detection_couleurs/vision");
//    map();
    while( couleurEquipe == -1 )
    {
	system("sleep 1");
	
    }
    vision(&M1, &M2, &M3, &M4, &M5, &M6, couleurEquipe);//On lance vision avec les adresses des mires
   
    while(!tirette_pulled) 
    { 
	system("sleep 2");
    } 
    
    //tirette_pulled passe à true par le tread scrutateur d'uart
    fprintf(stderr,"Tirette ok \n");

    printf("%d %d %d %d %d %d \n", M1, M2, M3, M4, M5, M6);
    sendUart = True; 

//On lance la carte
    map();

    pthread_cancel(tid_recept_uart);



    //attendre la terminaison du thread de scrutation
    //cette ligne peut être mise un peu plus haut (non obligatoire)
    (void)pthread_join(tid_recept_uart ,&ret);
    return 0; //fin du programme principal
}

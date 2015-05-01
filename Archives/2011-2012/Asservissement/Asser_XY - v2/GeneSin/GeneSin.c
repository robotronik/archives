#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

void quitte(void) {
	printf("Presser entree pour terminer.");
	while (getchar() == EOF);
	exit(0);
}

int main(void) {
    int fNum = open("sources/messageCommun.h", O_RDWR | O_BINARY);
    if (fNum < 0) {
		printf("Fichier \"sources/messageCommun.h\" absent => Fichiers non-modifies.\n");
		quitte();
	}
    
    char buf[1024];
    read(fNum, buf, 1024);
	
/****************************************** Lecture de TOUR ******************************************/
	char *pBuf = strstr(buf, "#define TOUR");
	if (!pBuf) {
		printf("\"#define TOUR\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf = strpbrk(pBuf, "123456789");
	if (!pBuf) {
		printf("Valeur de \"TOUR\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    char *pBuf2 = pBuf + strspn(pBuf, "1234567890");
    pBuf2[0] = 0;
    int tour = atoi(pBuf);
	printf("TOUR = %i\n", tour);
	
/*****************************************************************************************************/
    close(fNum);
    fNum = open("sources/asser.h", O_RDWR | O_BINARY);
    if (fNum < 0) {
		printf("Fichier \"sources/asser.h\" absent => Fichiers non-modifies.\n");
		quitte();
	}
    
    read(fNum, buf, 1024);
	
/****************************************** Lecture de METRE ******************************************/
	pBuf = strstr(buf, "#define METRE");
	if (!pBuf) {
		printf("\"#define METRE\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf = strpbrk(pBuf, "123456789");
	if (!pBuf) {
		printf("Valeur de \"METRE\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf2 = pBuf + strspn(pBuf, "1234567890");
    pBuf2[0] = 0;
	int metre = atoi(pBuf);
	printf("METRE = %i\n", metre);
/****************************************** Calcul de RAYON ******************************************/
    int rayon = 0xFFFFFFFF/3/metre;
	printf("RAYON = %i\n", rayon);
	if (rayon > USHRT_MAX) {
		printf("\n/!\\ RAYON > USHRT_MAX => Fichiers non-modifies.\n", rayon);
		quitte();
	}
	
/************************************* Ecriture de COEFF_ALPH_DELT *************************************/
	pBuf = strstr(++pBuf2, "#define COEFF_ALPH_DELT");
	if (!pBuf) {
		printf("\"#define COEFF_ALPH_DELT\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf = strpbrk(pBuf, "123456789");
	if (!pBuf) {
		printf("Valeur de \"COEFF_ALPH_DELT\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf2 = pBuf + strspn(pBuf, "1234567890");
	int coeff_alph_delt = tour/(2*M_PI);
	snprintf(pBuf, 1024 - (int)(pBuf - buf), "%-7i", coeff_alph_delt);
	pBuf[7] = ' ';
	printf("COEFF_ALPH_DELT = %i\n", coeff_alph_delt);
    lseek(fNum, pBuf - buf, SEEK_SET);
    write(fNum, pBuf, 7);
	
/***************************************** Ecriture de NB_SIN *****************************************/
	pBuf = strstr(++pBuf2, "#define NB_SIN");
	if (!pBuf) {
		printf("\"#define NB_SIN\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf = strpbrk(pBuf, "123456789");
	if (!pBuf) {
		printf("Valeur de \"NB_SIN\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf2 = pBuf + strspn(pBuf, "1234567890");
	int nb_sin = (tour+1)/2;
	snprintf(pBuf, 1024 - (int)(pBuf - buf), "%-7i", nb_sin);
	pBuf[7] = ' ';
	printf("NB_SIN = %i\n", nb_sin);
    lseek(fNum, pBuf - buf, SEEK_SET);
    write(fNum, pBuf, 7);
	
    close(fNum);
    
/************************************ Ecriture du tableau de sinus ************************************/
    fNum = open("sources/myMath.c", O_RDWR | O_BINARY);
    if (fNum < 0) {
		printf("Fichier \"sources/myMath.c\" absent.\n");
		quitte();
	}
    
    lseek(fNum, 600, SEEK_SET);
    read(fNum, buf, 1024);
	
	pBuf = strstr(buf, "const");
	if (!pBuf) {
		printf("\"const\" non trouve => Tableau de sinus non-modifie.\n");
		quitte();
	}
	pBuf = strstr(pBuf, "{");
	if (!pBuf) {
		printf("\"{\" du tableau de sinus non trouve => Tableau non-modifie\n");
		quitte();
	}
    pBuf ++;
    lseek(fNum, pBuf - buf + 600, SEEK_SET);
	int i;
	for (i=0; i<nb_sin; i++) {
		if (!(i % 10)) write(fNum, "\n\t", 2);
		long val = rayon * 2 * sin(i * M_PI_2 / nb_sin);
		val = (val >> 1) + val % 2;
		snprintf(buf, 1024, "%5i", val);
		buf[5] = ',';
		buf[6] = ' ';
		//printf("val = %i = \"%.*s\"\n", val, 7, buf);
		write(fNum, buf, 7);
	}
    lseek(fNum, -2, SEEK_CUR);
	sprintf(buf, "\n};");
	write(fNum, buf, 3);
	int taille = lseek(fNum, 0, SEEK_CUR);
	i = lseek(fNum, 0, SEEK_END) - 1;
	lseek(fNum, taille, SEEK_SET);
	char c = ' ';
	for (; i>taille; i--) write(fNum, &c, 1);
	write(fNum, "\n", 1);
    close(fNum);
	quitte();
}

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
    int fNum = open("sources/asser.h", O_RDWR | O_BINARY);
    if (fNum < 0) {
		printf("Fichier \"sources/asser.h\" absent => Fichiers non-modifies.\n");
		quitte();
	}
    
    char buf[1024];
    read(fNum, buf, 1024);
	
	char *pBuf = strstr(buf, "#define METRE");
	if (!pBuf) {
		printf("\"#define METRE\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf = strpbrk(pBuf, "123456789");
	if (!pBuf) {
		printf("Valeur de \"METRE\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    char *pBuf2 = pBuf + strspn(pBuf, "1234567890");
    pBuf2[0] = 0;
	int metre = atoi(pBuf);
	printf("METRE = %i ; ", metre);
    int rayon = 0xFFFFFFFF/3/metre;
	printf("RAYON = %i ; ", rayon);
	if (rayon > USHRT_MAX) {
		printf("\n/!\\ RAYON > USHRT_MAX => Fichiers non-modifies.\n", rayon);
		quitte();
	}
	
	pBuf = strstr(++pBuf2, "#define TOUR");
	if (!pBuf) {
		printf("\"#define TOUR\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf = strpbrk(pBuf, "123456789");
	if (!pBuf) {
		printf("Valeur de \"TOUR\" non trouve => Fichiers non-modifies.\n");
		quitte();
	}
    pBuf2 = pBuf + strspn(pBuf, "1234567890");
    pBuf2[0] = 0;
    int tour = atoi(pBuf);
	printf("TOUR = %i ; ", tour);
	
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
	printf("NB_SIN = %i => \"%.*s\"\n", nb_sin, 7, pBuf);
    lseek(fNum, pBuf - buf, SEEK_SET);
    write(fNum, pBuf, 7);
	
    close(fNum);
    
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
	//for (i=0; i<4; i++) {
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
	sprintf(buf, "};\n");
	write(fNum, buf, 3);
	int taille = lseek(fNum, 0, SEEK_CUR);
	i = lseek(fNum, 0, SEEK_END);
	lseek(fNum, taille, SEEK_SET);
	//SetEndOfFile(fNum);						// Windows
	//_chsize_s(fNum, taille);					// Windows
	char c = ' ';
	for (; i>taille; i--) write(fNum, &c, 1);
    close(fNum);
	//truncate("sources/myMath.c", taille);		// Unix
	quitte();
}

#include "Display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *fName = "Input.bin";
	unsigned int start = 0, nbEchantOut = 0;
	int i;
	for (i = 1; i < argc; i ++) {
		if (argv[i][1] == '=') {
			if (argv[i][0] == 'i') {
				fName = argv[i] + 2;
			} else if (argv[i][0] == 's') {
				start = atoi(argv[i] + 2);
			} else if (argv[i][0] == 'e') {
				nbEchantOut = atoi(argv[i] + 2);
			}
		}
	}
	
	strrchr(argv[0], '/')[1] = 0;
	
	FILE *f = fopen(fName, "r");
	if (f == NULL) {
		printf("%s does not exist.\nUse \"i=<name>\" to choose an input file.", fName);
		return -1;
	}
	unsigned int nbEchant;
	fread(&nbEchant, sizeof(nbEchant), 1, f);
	
	if (start < 0 || start > nbEchant - 1) start = 0;
	if (nbEchantOut == 0 || nbEchantOut < start || nbEchantOut > nbEchant - 1) nbEchantOut = nbEchant - 1;
	printf("Echantillons %u a %u\n", start, nbEchantOut);
	nbEchantOut = nbEchantOut - start + 1;
	
	/** Affichage **/
	char name[32];
	long long *varTab = (long long *) malloc(nbEchant * sizeof(*varTab));
	int varId = 0;
	while (fscanf(f, "%s ", name) == 1) {
		char isSigned;
		fread(&isSigned, sizeof(isSigned), 1, f);
		fread(varTab, sizeof(*varTab), nbEchant, f);
		
		display(argv[0], varId, name, varTab + start, isSigned, nbEchantOut);
		varId ++;
	}
	free(varTab);
	
	return 0;
}

#include "../../PSoC/CommandeAsser.cydsn/init.h"
#include "AsserGlue.h"
#include "PSoCGlue.h"
#include "Acquirer.h"
#include "Vars.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	char *fIn = "Input.bin", *fOut = "Output.bin";
	int i;
	for (i = 1; i < argc; i ++) {
		if (argv[i][1] == '=') {
			if (argv[i][0] == 'i') {
				fIn = argv[i] + 2;
			} else if (argv[i][0] == 'o') {
				fOut = argv[i] + 2;
			}
		}
	}
	
	AsserInit();
	PSoCinit();
	
	/** Acquisition **/
	long long *varTabTab[NB_VARS] = {NULL};
	unsigned int nbBoucle = acquire(fIn, varTabTab);
	
	/** Sortie **/
	int varId;
	FILE *f = fopen(fOut, "wb");
	fwrite(&nbBoucle, sizeof(nbBoucle), 1, f);
	fIn[0] = ' ';
	for (varId=0; varId<NB_VARS; varId++) {
		fwrite(getVars()[varId].name, strlen(getVars()[varId].name), 1, f);
		fwrite(fIn, 1, 1, f);
		char isSigned = getVars()[varId].isSigned;
		fwrite(&isSigned, 1, 1, f);
		fwrite(varTabTab[varId], sizeof(*(varTabTab[varId])), nbBoucle, f);
	}
	fclose(f);
	
	freeAcquisitions(varTabTab);
	printf("Presser entree pour terminer.");
	while (getchar() == EOF);
	return 0;
}

#include "PSoCGlue.h"
#include "AsserGlue.h"
#include "Vars.h"
#include <stdio.h>
#include <stdlib.h>

#define NB_DATA		2
#define DATA_LEN	1				// [octets]
#define DATA_MAX	(2^^8*DATA_LEN)	// [bits]

long long getSetMin(unsigned int varId, boolean isSigned, boolean isFirstLoad, long long loadVal) {
	static long long tabMin[NB_VARS];
	
	long long ret = tabMin[varId];
	if (isSigned) {
		if (isFirstLoad || loadVal < tabMin[varId]) tabMin[varId] = loadVal;
	} else {
		if (isFirstLoad || (unsigned long long) loadVal < (unsigned long long) tabMin[varId]) tabMin[varId] = loadVal;
	}
	return ret;
}

long long getSetMax(unsigned int varId, boolean isSigned, boolean isFirstLoad, long long loadVal) {
	static long long tabMax[NB_VARS];
	
	long long ret = tabMax[varId];
	if (isSigned) {
		if (isFirstLoad || loadVal > tabMax[varId]) tabMax[varId] = loadVal;
	} else {
		if (isFirstLoad || (unsigned long long) loadVal > (unsigned long long) tabMax[varId]) tabMax[varId] = loadVal;
	}
	return ret;
}

static long long acquireNextValOfVar(unsigned int varId, unsigned int t) {
	var_t var = getVars()[varId];
	long long val;
	
	if (var.isSigned) {
		switch (var.size) {
		case 1:
				val = *(char *)(var.ptr);
			break;
		case 2:
				val = *(short *)(var.ptr);
			break;
		case 4:
				val = *(long *)(var.ptr);
			break;
		case 8:
				val = *(long long *)(var.ptr);
		}
	} else {
		switch (var.size) {
		case 1:
				val = *(unsigned char *)(var.ptr);
			break;
		case 2:
				val = *(unsigned short *)(var.ptr);
			break;
		case 4:
				val = *(unsigned long *)(var.ptr);
			break;
		case 8:
				val = *(unsigned long long *)(var.ptr);
		}
	}
	getSetMin(varId, var.isSigned, !t, val);
	getSetMax(varId, var.isSigned, !t, val);
	
	return val;
}

unsigned int acquire(char *fName, long long *varTabTab[NB_VARS]) {
	unsigned int varTabsSize = 0;
	int numData = 0;
	int reception = 1;
	unsigned char vitSave;
	unsigned char byte;
	unsigned int t = 0;
	FILE *fr = fopen(fName, "rb");
	if (fr == NULL) {
		printf("%s does not exist.", fName);
	}
	while (fread(&byte, 1, 1, fr)) {
		if (!byte) {
			if (!fread(&byte, 1, 1, fr)) break;	// commande ou donnée 0x00
			if (byte) {							// commande :
				if (byte == 3)						// envoi
					reception = 0;
				else if (byte == 4)					// reception
					reception = 1;
				else if (byte == 5)					// connection
					fread(&byte, 1, 1, fr);				// Passe le N° de port
				continue;
			}
		}
		if (!reception) {
			PSoC_UART_fromXBeeSetRXByte(byte);
		} else {
			numData ++; numData %= NB_DATA;
			if (!numData) {
				execAsser(vitSave, byte);
				
				/** Realloc **/
				int varId;
				if (varTabsSize == t) {
					varTabsSize += 256;
					for (varId=0; varId<NB_VARS; varId++) {
						varTabTab[varId] = (long long *) realloc(varTabTab[varId], varTabsSize * sizeof(**varTabTab));
					}
				}
				
				/** Enregistrement **/
				for (varId=0; varId<NB_VARS; varId++)
					varTabTab[varId][t] = acquireNextValOfVar(varId, t);
				
				t ++;
				if (!(t % 100)) printf("Acquiring: t=%d\n", t);
			} else
				vitSave = byte;
		}
	}
	printf("Acquiring: t=%d (END)\n", t);
	fclose(fr);
	return t;
}

void freeAcquisitions(long long **acquisitions) {
	int varId;
	for (varId=0; varId<NB_VARS; varId++) {
		free(acquisitions[varId]);
	}
}

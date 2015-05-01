#include "PSoCGlue.h"
#include "AsserGlue.h"
#include "Vars.h"
#include "../../sources/XYAngle.h"
#include "../../sources/messageCommun.h"
#include "../../PSoC/CommandeAsser.cydsn/init.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

unsigned int get_x(void) {
	unsigned int x = (unsigned long) posX.part.high*3000/0xFFFF;
    if(!isRed)
        x = 3000 - x;
	return x;
}

unsigned int get_y(void) {
	return 2000 - (unsigned int) ((unsigned long) posY.part.high*3000/0xFFFF);
}

unsigned int get_angle(void) {
	unsigned int a = (unsigned long) posAlpha*360/TOUR;
    if(isRed) {
    	a = 360 - a;
	} else {
    	a = (180 + a) % 360;
	}
	return a;
}

long long *varTabTab[NB_VARS];
unsigned int nbBoucleAsser = 0;
static unsigned int varTabsSize = 0;
void acquire(void) {
	/** Realloc **/
	int varId;
	if (varTabsSize == nbBoucleAsser) {
		varTabsSize += 256;
		for (varId=0; varId<NB_VARS; varId++) {
			varTabTab[varId] = (long long *) realloc(varTabTab[varId], varTabsSize * sizeof(**varTabTab));
		}
	}
	
	/** Enregistrement **/
	for (varId=0; varId<NB_VARS; varId++)
		varTabTab[varId][nbBoucleAsser] = acquireNextValOfVar(varId, nbBoucleAsser);
	
	nbBoucleAsser ++;
	
	if (!(nbBoucleAsser % 400))
		printf("\E[32m%u %u %u\n\E[m", get_x(), get_y(), get_angle());
}

void freeAcquisitions(void) {
	int varId;
	
	for (varId=0; varId<NB_VARS; varId++) {
		free(varTabTab[varId]);
	}
	varTabsSize = 0;
	nbBoucleAsser = 0;
}

#ifndef _ACQUIRER_H
#define _ACQUIRER_H

#include "Vars.h"

extern long long *varTabTab[NB_VARS];
extern unsigned int nbBoucleAsser;

void acquire(void);
void freeAcquisitions(void);
long long getSetMin(unsigned int varId, boolean isSigned, boolean isFirstLoad, long long loadVal);
long long getSetMax(unsigned int varId, boolean isSigned, boolean isFirstLoad, long long loadVal);

#endif

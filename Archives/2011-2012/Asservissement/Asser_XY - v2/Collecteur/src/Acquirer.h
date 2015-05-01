#ifndef _ACQUIRER_H
#define _ACQUIRER_H

#include "Vars.h"

unsigned int acquire(char *fName, long long *varTabTab[NB_VARS]);
void freeAcquisitions(long long **acquisitions);
long long getSetMin(unsigned int varId, boolean isSigned, boolean isFirstLoad, long long loadVal);
long long getSetMax(unsigned int varId, boolean isSigned, boolean isFirstLoad, long long loadVal);

#endif

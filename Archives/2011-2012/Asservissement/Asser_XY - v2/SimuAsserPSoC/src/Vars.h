#ifndef _VARS_H
#define _VARS_H

#include "../../sources/Boolean.h"

#define NB_VARS 33

typedef struct {
	const void *ptr;
	const unsigned int size;
	const char name[32];
	const boolean isSigned;
} var_t;

var_t *getVars();

#endif

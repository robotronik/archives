#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define GRID_PRIM_COLOR		"dodgerblue"
#define GRID_SEC_COLOR		"rgb(56%, 78%, 100%)"	// Demi dodgerblue
#define GRID_YNULL_COLOR	"red"
#define TEXT_COLOR			GRID_PRIM_COLOR

static inline unsigned long long val2y(unsigned long long offset, long long val) {
	return offset - (unsigned long long) val;
}

#define SVG_VAR1	"  <polyline points=\""
#define SVG_VAR2	"\"\n  style=\"fill:none;stroke:black\"/>\n"

static void displayVar(FILE *f, unsigned int nbEchant, unsigned int reductionX, const long long *varTab, char isSigned, unsigned long long offset, float reductionY) {
	fwrite(SVG_VAR1, strlen(SVG_VAR1), 1, f);
	
	unsigned int t;
	union {
		long long s;
		unsigned long long u;
	} echant;
	echant.s = 0;
	for (t=0; t<nbEchant; t++) {
		//if (t && !(t % 10)) fprintf(f, "\n");
		if (isSigned)	echant.s += varTab[t] / reductionY;
		else			echant.u += (unsigned long long) varTab[t] / reductionY;
		if (!((t+1) % reductionX)) {
			if (isSigned)
				fprintf(f, "%u,%llu ", t/reductionX, val2y(offset, echant.s/reductionX));
			else
				fprintf(f, "%u,%llu ", t/reductionX, val2y(offset, echant.u/reductionX));
			echant.s = 0;
		}
	}
	
	fwrite(SVG_VAR2, strlen(SVG_VAR2), 1, f);
}

#define SVG_LINE1	"  <line x1=\""
#define SVG_LINE2	"\" y1=\""
#define SVG_LINE3	"\" x2=\""
#define SVG_LINE4	"\" y2=\""
#define SVG_LINE5	"\"\n  style=\"stroke:"
#define SVG_LINE6	";stroke-width:1\"/>\n"

static void displayLine(FILE *f, unsigned long long x1, unsigned long long y1, unsigned long long x2, unsigned long long y2, const char *color) {
	fwrite(SVG_LINE1, strlen(SVG_LINE1), 1, f);
	fprintf(f, "%llu", x1);
	fwrite(SVG_LINE2, strlen(SVG_LINE2), 1, f);
	fprintf(f, "%llu", y1);
	fwrite(SVG_LINE3, strlen(SVG_LINE3), 1, f);
	fprintf(f, "%llu", x2);
	fwrite(SVG_LINE4, strlen(SVG_LINE4), 1, f);
	fprintf(f, "%llu", y2);
	fwrite(SVG_LINE5, strlen(SVG_LINE5), 1, f);
	fwrite(color, strlen(color), 1, f);
	fwrite(SVG_LINE6, strlen(SVG_LINE6), 1, f);
}

#define SVG_TEXT1	"  <text x=\""
#define SVG_TEXT2	"\" y=\""
#define SVG_TEXT3	"\" fill=\"" TEXT_COLOR "\">"
#define SVG_TEXT4	"</text>\n"

static void displayInt(FILE *f, unsigned long long x, unsigned long long y, long long val, char isSigned) {
	fwrite(SVG_TEXT1, strlen(SVG_TEXT1), 1, f);
	fprintf(f, "%llu", x);
	fwrite(SVG_TEXT2, strlen(SVG_TEXT2), 1, f);
	fprintf(f, "%llu", y);
	fwrite(SVG_TEXT3, strlen(SVG_TEXT3), 1, f);
	if (isSigned) fprintf(f, "%lld", val);
	else fprintf(f, "%llu", (unsigned long long) val);
	fwrite(SVG_TEXT4, strlen(SVG_TEXT4), 1, f);
}

#define SVG_MARGE		4
#define SVG_TEXT_HEIGHT	12

static void displayHGridLine(FILE *f, unsigned long long offset, long long val, float reductionY, char isSigned, unsigned int nbEchant) {
	unsigned long long y = val2y(offset, val);
	char *color;
	if (val) color = GRID_PRIM_COLOR;
	else color = GRID_YNULL_COLOR;
	displayLine(f, 0, y, nbEchant, y, color);
	displayInt(f, SVG_MARGE, y - SVG_MARGE, val * reductionY, isSigned);
}

static void displayVGridLine(FILE *f, long long val, unsigned int reductionX, unsigned long long offset, long long min) {
	unsigned long long bottom = val2y(offset, min);
	displayLine(f, val, 0, val, bottom, GRID_SEC_COLOR);
	displayInt(f, val + SVG_MARGE, bottom - SVG_MARGE, val * reductionX, 0);
}

static void displayGrid(FILE *f, unsigned int nbEchant, unsigned int reductionX, unsigned long long offset, long long min, long long max, float reductionY, char isSigned) {
	if (isSigned) {
		long long i;
		if (min < 0) min = min/50*50 - 50;
		else min = min/50*50;
		for (i=min; i<=max; i+=10) {
			if (i % 50) {
				unsigned long long y = val2y(offset, i);
				displayLine(f, 0, y, nbEchant, y, GRID_SEC_COLOR);
			} else displayHGridLine(f, offset, i, reductionY, 1, nbEchant);
		}
	} else {
		unsigned long long i;
		min = (unsigned long long) min/50*50;
		for (i=(unsigned long long) min; i<=(unsigned long long) max; i+=10) {
			if (i % 50) {
				unsigned long long y = val2y(offset, i);
				displayLine(f, 0, y, nbEchant, y, GRID_SEC_COLOR);
			} else displayHGridLine(f, offset, i, reductionY, 0, nbEchant);
		}
	}
	unsigned int i;
	for (i=100; i<nbEchant/reductionX; i+=100) {
		displayVGridLine(f, i, reductionX, offset, min);
	}
}

#define OUT_PATH	"Outputs/"
#define OUT_EXT		".svg"

#define SVG_START1	"<?xml version=\"1.0\" standalone=\"yes\"?>\n<svg width=\""
#define SVG_START2	"\" height=\""
#define SVG_START3	"\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
#define SVG_END		"</svg>"

#define SVG_X_MAX	13400
#define SVG_Y_MAX	6300

void display(const char *path, int varId, const char *name, const long long *varTab, char isSigned, unsigned int nbEchant) {
	printf("Displaying: %02u %s\n", varId, name);
	const unsigned long long *uVarTab = varTab;
	
	union {
		long long s;
		unsigned long long u;
	} min, max;
	if (isSigned) {
		min.s = LLONG_MAX;
		max.s = LLONG_MIN;
	} else {
		min.u = ULLONG_MAX;
		max.u = 0;
	}
	int i;
	for (i = 0; i < nbEchant; i ++) {
		if (isSigned) {
			if (varTab[i] < min.s) min.s = varTab[i];
			if (varTab[i] > max.s) max.s = varTab[i];
		} else {
			if (uVarTab[i] < min.u) min.u = uVarTab[i];
			if (uVarTab[i] > max.u) max.u = uVarTab[i];
		}
	}
	
	char *fName = (char *) malloc(strlen(path) + strlen(OUT_PATH) + 3 + strlen(name) + strlen(OUT_EXT) + 1);
	strcpy(fName, path);
	strcat(fName, OUT_PATH);
	char varIdStr[4];
	sprintf(varIdStr, "%02u ", varId);
	strcat(fName, varIdStr);
	strcat(fName, name);
	strcat(fName, OUT_EXT);
	FILE *f = fopen(fName, "w");
	
	/** En-tête **/
	fwrite(SVG_START1, strlen(SVG_START1), 1, f);
	unsigned int reductionX = nbEchant / SVG_X_MAX + 1;
	fprintf(f, "%u", nbEchant / reductionX);
	fwrite(SVG_START2, strlen(SVG_START2), 1, f);
	float reductionY;
	if (isSigned) {
		reductionY = (float) (max.s - min.s) / (SVG_Y_MAX - 1 - 2*SVG_MARGE - SVG_TEXT_HEIGHT);
		if (reductionY < 0.1) reductionY = 0.1;
		min.s /= reductionY;
		max.s /= reductionY;
		if (min.s < 0)
			fprintf(f, "%llu", (unsigned long long) (max.s - min.s/50*50 + 50 + 1 + 2*SVG_MARGE + SVG_TEXT_HEIGHT));
		else
			fprintf(f, "%llu", (unsigned long long) (max.s - min.s/50*50 + 1 + 2*SVG_MARGE + SVG_TEXT_HEIGHT));
	} else {
		reductionY = (float) (max.u - min.u) / (SVG_Y_MAX - 1 - 2*SVG_MARGE - SVG_TEXT_HEIGHT);
		if (reductionY < 0.02) reductionY = 0.02;
		min.u /= reductionY;
		max.u /= reductionY;
		fprintf(f, "%llu", max.u - min.u/50*50 + 1 + 2*SVG_MARGE + SVG_TEXT_HEIGHT);
	}
	fwrite(SVG_START3, strlen(SVG_START3), 1, f);
	
	unsigned long long offset = 2*SVG_MARGE + SVG_TEXT_HEIGHT;
	if (!isSigned) offset = max.u + 2*SVG_MARGE + SVG_TEXT_HEIGHT;
	else if (max.s > 0) offset = max.s + 2*SVG_MARGE + SVG_TEXT_HEIGHT;
	
	/** Grille **/
	displayGrid(f, nbEchant, reductionX, offset, min.s, max.s, reductionY, isSigned);
	
	/** Courbe **/
	displayVar(f, nbEchant, reductionX, varTab, isSigned, offset, reductionY);
	
	fwrite(SVG_END, strlen(SVG_END), 1, f);
	fclose(f);
}

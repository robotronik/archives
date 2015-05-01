#include "../../PSoC/CommandeAsser.cydsn/init.h"
#include "../../PSoC/CommandeAsser.cydsn/test_asser.h"
#include "../../sources/asser.h"
#include "../../sources/messageAsser.h"
#include "AsserGlue.h"
#include "PSoCGlue.h"
#include "Canaux.h"
#include "Acquirer.h"
#include "Vars.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

enum {IDLE, CMD, SLEEP, OLD_CMD};
#define BUF_SIZE	256

int strParse(char *strOut, int len) {
	char *outPtr = strOut;
	static char strIn[BUF_SIZE] = {0};
	static char *cmdStr;
	static unsigned int cmdStrSize;
	static unsigned int cmdStrLen;
	static int i = 0, state = IDLE;
	
	if (state == SLEEP) {
		int t;
		sscanf(cmdStr + 1, "%d", &t);
		free(cmdStr);
		usleep(t * 1000);
		i ++;
		state = IDLE;
	} else if (state == OLD_CMD) {
		int nbByte = cmdStr[0] - '0';
		if (nbByte > len) return 0;
		
		char sscanfFormat[5] = "%ll ";
		sscanfFormat[3] = cmdStr[1];
		long long val;
		if (sscanf(cmdStr + 2, sscanfFormat, &val, 8) != 1) {
			free(cmdStr);
			return -1;
		}
		free(cmdStr);
		
		for (nbByte --; nbByte>=0; nbByte --) {
			*outPtr = val >> (8*nbByte);
			outPtr ++;
		}
		i ++;
		state = IDLE;
	}
	
	if (!strIn[i]) {
		char format[8];
		sprintf(format, "%%%ds", BUF_SIZE - 1);
		while (scanf(format, strIn) < 1);
		i = 0;
	}
	for (; strIn[i]; i++) {
		if (state == CMD) {
			if (strIn[i] == '%') {
				if (!cmdStrLen) {
					*outPtr = '%';
					if (outPtr - strOut == len) return len;
					outPtr ++;
					state = IDLE;
					continue;
				}
				if (cmdStrLen == cmdStrSize) {
					cmdStrSize ++;
					cmdStr = (char *) realloc(cmdStr, cmdStrSize);
				}
				cmdStr[cmdStrLen] = 0;
				cmdStrLen ++;
				switch (cmdStr[0]) {
				case 'p':
					if (cmdStrLen < 2 
					|| strspn(cmdStr + 1, "1234567890") < 1) {
						free(cmdStr);
						return -1;
					}
					state = SLEEP;
					return outPtr - strOut;
				case 't':
					Depart_Var(TRUE);
					break;
				case 'c':
					if (cmdStrLen < 2 
					|| strspn(cmdStr + 1, "rp") < 1) {
						free(cmdStr);
						return -1;
					}
					Couleur_Set(cmdStr[1]);
					break;
				default:
					if (cmdStrLen < 3 
					|| strspn(cmdStr, "12345678") != 1 
					|| strspn(cmdStr + 1, "xXduieEfgGo") != 1) {
						free(cmdStr);
						return -1;
					}

					int nbByte = cmdStr[0] - '0';
					if (outPtr - strOut + nbByte > len) {
						state = OLD_CMD;
						return outPtr - strOut;
					}
					
					char sscanfFormat[5] = "%ll ";
					sscanfFormat[3] = cmdStr[1];
					long long val;
					if (sscanf(cmdStr + 2, sscanfFormat, &val, 8) != 1) {
						free(cmdStr);
						return -1;
					}
					free(cmdStr);
					
					for (nbByte --; nbByte>=0; nbByte --) {
						*outPtr = val >> (8*nbByte);
						outPtr ++;
					}
				}
				state = IDLE;
				continue;
			}
			if (cmdStrLen == cmdStrSize) {
				cmdStrSize += 16;
				cmdStr = (char *) realloc(cmdStr, cmdStrSize);
			}
			cmdStr[cmdStrLen] = strIn[i];
			cmdStrLen ++;
		} else if (strIn[i] == '%') {
			state = CMD;
			cmdStrLen = 0;
			cmdStrSize = 0;
			cmdStr = NULL;
		} else {
			*outPtr = strIn[i];
			if (outPtr - strOut == len) return len;
			outPtr ++;
		}
	}
	if (i < BUF_SIZE - 1 && state == CMD) return -1;
	strIn[0] = 0;
	return outPtr - strOut;
}

void UART_handler(int jsaipas) {
	unsigned char byte;
	char action = 1;
	pthread_t pthread;
	
	pthread = pthread_self();
	if (pthread == asser_pthread) {
		while (UART_get(PSoC_to_asser, &byte)) {
			Asser_UART_handler(byte);
		}
	} else if (pthread == PSoC_pthread) {
		while (action) {
			action = 0;
			if (UART_get(asser_to_PSoC, &byte)) {
				PSoC_UART_from_asser_handler(byte);
				action = 1;
			}
			if (UART_get(PC_to_PSoC, &byte)) {
				PSoC_UART_from_PC_handler(byte);
				action = 1;
			}
		}
	}
}

int main(int argc, char **argv) {
	char *fName;
	if (argc > 1)	fName = argv[1];
	else			fName = "Output.bin";

	setbuf(stdout, NULL);
	printf("Launched\n");
	PSoC_to_asser = UART_canal_init();
	asser_to_PSoC = UART_canal_init();
	PC_to_PSoC = UART_canal_init();
	main_pthread = pthread_self();
	
	struct sigaction sigact;
	sigact.sa_handler = UART_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
	sigaction(SIGUSR1, &sigact, NULL);
	sigset_t sigset;
	sigaddset(&sigset, SIGUSR1);
	pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
	
	char state = 0;
	pthread_create(&PSoC_pthread, NULL, PSoCThread, &state);
	pthread_create(&asser_pthread, NULL, asserThread, &state);
	
	while (1) {
		char str[256];
		int len = strParse(str, 256);
		int i;
		if (len < 0) break;
		for (i=0; i<len; i++) {
			UART_put(PC_to_PSoC, str[i]);
			pthread_kill(PSoC_pthread, SIGUSR1);
			usleep(10000);
		}
	}
	state = 1;
	
	pthread_join(asser_pthread, NULL);
	UART_canal_destroy(PSoC_to_asser);
	UART_canal_destroy(asser_to_PSoC);
	UART_canal_destroy(PC_to_PSoC);
	
	/** Sortie **/
	state = ' ';
	int varId;
	FILE *f = fopen(fName, "wb");
	fwrite(&nbBoucleAsser, sizeof(nbBoucleAsser), 1, f);
	for (varId=0; varId<NB_VARS; varId++) {
		fwrite(getVars()[varId].name, strlen(getVars()[varId].name), 1, f);
		fwrite(&state, 1, 1, f);
		char isSigned = getVars()[varId].isSigned;
		fwrite(&isSigned, 1, 1, f);
		fwrite(varTabTab[varId], sizeof(*(varTabTab[varId])), nbBoucleAsser, f);
	}
	fclose(f);
	
	freeAcquisitions();
	return 0;
}

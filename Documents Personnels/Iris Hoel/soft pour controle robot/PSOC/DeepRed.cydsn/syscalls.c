/**
* @file
* @author Wilbur Harvey
* @version 0.9.0.0
* @date 2011-Mar-03
*
* @brief SYSCALLS, Replacement for newlib syscalls.c
*
* @section Description
* This file contains replacement for the arm newlib syscalls.c
* This file was copied from newlib-syscalls.c and modified for PSoC
*
* @section History
* 2011-Mar-03 wnh Started file
*/


/**
* Includes
*/
#include <device.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

// Function not supported, so return ERROR
int _open(const char *name, int flags, int mode) {
errno = ENOENT;
return -1;
}

// Function not supported, return dummy values that won't break anything
int _close(int file) {
if (file == 1 || file == 2) {
return 0;
}
errno = EBADF;
return -1;
}

// Function not supported, return dummy values that won't break anything
int _isatty(int file) {
if (file >= 0 && file <= 2) {
return 1;
}
return 0;
}

// Currently only spporting a read of length 1, getchar will read from UART1
// Blocking
int _read(int file, char *ptr, int len){
while(1) {
if(UART_XBee_GetRxBufferSize() > 0) {
ptr[0] = (uint8)UART_XBee_GetByte(); // must use GetByte so we can receive a \0 character
break;
}
}
return 1;
}

// Allows printf, putchar etc. to output to UART_XBee
int _write(int file, const char *ptr, int len){
int sent = -1;
if (file == 1 || file == 2) {
UART_XBee_PutArray((uint8 *)ptr, (uint8)len);
sent = len;
}
return sent;
}

// Function not supported, return OK
int _lseek(int file, int ptr, int dir) {
return 0;
}

// Function not supported, return dummy values that won't break anything
int _fstat(int file, struct stat *st) {
if (file >= 0 && file <= 2) {
st->st_mode = S_IFCHR;
return 0;
}
errno = EBADF;
return -1;
}

// Function not supported, return ERROR
int _stat(char *file, struct stat *st) {
errno = ENOENT;
return -1;
}

// Function not supported, return dummy values that won't break anything
int fsync(int fd) {
if (fd == 1 || fd == 2) {
return 0;
}
if (fd == 0) return 0;
errno = EBADF;
return -1;
}


// Supported as hang the system
void _exit(int status) {
while(1);
}

// Supported as hang the system
void _abort() {
while(1);
}

// Supported as hang the system
void _kill() {
while(1);
}

// Always returns a PID of 1
pid_t _getpid(void) {
return 1;
}
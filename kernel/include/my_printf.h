#include <stdio.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include "trap.h"

int read(int, char *, int);
int write(int, char *, int);
off_t lseek(int, off_t, int);
int close(int);
int fstat(int, struct stat *);
int isatty(int);

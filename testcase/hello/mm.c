#include <stdio.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <malloc.h>
 
#include "trap.h"
 
int __attribute__((__noinline__))
syscall(int id, ...) {
    int ret;
    int *args = &id;
    asm volatile("int $0x80": "=a"(ret) : "a"(args[0]), "b"(args[1]), "c"(args[2]), "d"(args[3]));
    return ret;
}
 
int read(int fd, char *buf, int len) {
    nemu_assert(0);
    return 0;
}
 
int write(int fd, char *buf, int len) {
    return syscall(SYS_write, fd, buf, len); 
}
 
off_t lseek(int fd, off_t offset, int whence) {
    nemu_assert(0);
    return 0;
}
 
void *sbrk(int incr) {
    extern char end;        /* Defined by the linker */
    static char *heap_end;
    char *prev_heap_end;
 
    if (heap_end == 0) {
        heap_end = &end;
    }
    prev_heap_end = heap_end;
 
    if( syscall(SYS_brk, heap_end + incr) == 0) {
        heap_end += incr;
    }
 
    return prev_heap_end;
}
 
int close(int fd) {
    nemu_assert(0);
    return 0;
}
 
int fstat(int fd, struct stat *buf) {
    return 0;
}
 
int isatty(int fd) {
    nemu_assert(0);
    return 0;
}
 
int main() {
    int i;
    char *s;
    char *p;
    for (i = 0; i < 100; i ++) {
        s = (char *)calloc(1, 165000);
        p = (char *)calloc(1, 8);
        free(s);
        free(p);
    }
    return 0;
}

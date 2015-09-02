#include "x86.h"
#include "trap.h"
#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
void serial_printc(char);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
    mm_brk(tf->ebx);
#endif
    tf->eax = 0;
}

int print(void *buf, int len)
{
    char *str = buf;
    int i;
    for (i = 0; i < len; i ++) {
        serial_printc(str[i]);
    }
    return len;
}

int fs_write(int fd, void *buf, int len);
int fs_open(char *filename, int flag);
int fs_read(int fd, void *buf, int len);
int fs_lseek(int fd, int offset, int whence);
int fs_close(int fd);
void do_syscall(TrapFrame *tf)
{
    switch(tf->eax) {
        /* The ``add_irq_handle'' system call is artificial. We use it to 
         * let user program register its interrupt handlers. But this is 
         * very dangerous in a real operating system. Therefore such a 
         * system call never exists in GNU/Linux.
         */
        case 0: add_irq_handle(tf->ebx, (void*)tf->ecx); break;

        case SYS_brk: sys_brk(tf); break;

        /* TODO: Add more system calls. */

        case SYS_write:
                      tf->eax = fs_write(
                              tf->ebx,
                              (void *)(tf->ecx),
                              tf->edx);
                      break;
        case SYS_open:
                      tf->eax = fs_open((char *)tf->ebx, tf->ebx);
                      break;
        case SYS_read:
                      tf->eax = fs_read(
                              tf->ebx,
                              (void *)(tf->ecx),
                              tf->edx);
                      break;
        case SYS_lseek:
                      tf->eax = fs_lseek(tf->ebx, tf->ecx, tf->edx);
                      break;
        case SYS_close:
                      tf->eax = fs_close(tf->ebx);
                      break;
        default: 
                      panic("Unhandled system call: id = %d", tf->eax);
    }
}

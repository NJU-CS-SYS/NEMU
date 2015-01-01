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

int print(uint32_t buf, int len)
{
	char *str = (char *)buf;
	int i;
	for (i = 0; i < len; i ++) {
		serial_printc(str[i]);
	}
	return len;
}

void do_syscall(TrapFrame *tf) {
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
					  // asm volatile(".byte 0x82": :"a"(2), "c"(tf->ecx), "d"(tf->edx));
					  // tf->eax has been modified as return value
					  tf->eax = print(tf->ecx, tf->edx);					  
					  break;
		
		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

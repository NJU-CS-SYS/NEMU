#include <setjmp.h>
#include "common.h"

#include "cpu/reg.h"
#include "idt.h"

extern jmp_buf jbuf;

void raise_intr(uint8_t NO)
{
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ``NO'' to index the IDT.
	 */	 
	lnaddr_t idt_addr = cpu.idtr.base; 
	size_t size = sizeof(GateDesc);
	Log("idt_addr %x", idt_addr);
	Log("size %x", size);
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}

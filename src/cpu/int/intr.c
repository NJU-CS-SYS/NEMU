#include <setjmp.h>
#include "common.h"

#include "cpu/reg.h"
#include "idt.h"
#include "cpu/segment.h"

extern jmp_buf jbuf;

void raise_intr(uint8_t NO)
{
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ``NO'' to index the IDT.
	 */	 
	lnaddr_t idt_addr = cpu.idtr.base; 
	size_t size = sizeof(GateDesc);
	GateDesc desc;
	*(uint32_t *)(&desc) = hwaddr_read(idt_addr + NO * size, 4);
	*((uint32_t *)(&desc) + 1) = hwaddr_read(idt_addr + NO * size + 4, 4);
	test((desc.type & 0x4) == 0x4, "failed in bit check");
	Log("idt_addr %x", idt_addr);
	Log("size %x", size);
	Log("desc\n%x\n%x", *(uint32_t *)(&desc), *((uint32_t *)(&desc) + 1));
	test(0, "计划报废desu");
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}

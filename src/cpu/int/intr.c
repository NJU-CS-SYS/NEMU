#include <setjmp.h>
#include "common.h"

#include "cpu/reg.h"
#include "idt.h"
#include "cpu/segment.h"
#include "stack.h"

extern jmp_buf jbuf;

void raise_intr(uint8_t NO)
{
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ``NO'' to index the IDT.
	 */	 

	/* Get gate descriptor */
	lnaddr_t idt_addr = cpu.idtr.base; 
	size_t size = sizeof(GateDesc);
	GateDesc desc;
	*(uint32_t *)(&desc) = hwaddr_read(idt_addr + NO * size, 4);
	*((uint32_t *)(&desc) + 1) = hwaddr_read(idt_addr + NO * size + 4, 4);

	/* Store current infomation */
	push(cpu.eflags);
	test(0, "计划报废desu");
	push(cpu.cs);
	push(cpu.eip);

	/* Jump */
	cpu.cs = desc.segment;
	cpu.eip = (desc.offset_31_16 << 16) | (desc.offset_15_0);

	/* Debug */
	test(desc.present, "failed in checking present bit of gate descriptor");
	test((desc.type & 0x4) == 0x4, "failed in bit check");
	Log("idt_addr %08x", idt_addr);
	Log("size %x", size);
	Log("desc\n%08x\n%08x", 
			*(uint32_t *)(&desc), *((uint32_t *)(&desc) + 1));
	Log("new cs %x", cpu.cs);
	Log("new eip %x", cpu.eip);

	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}

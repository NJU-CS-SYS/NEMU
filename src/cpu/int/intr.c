#include <setjmp.h>
#include "common.h"

#include "cpu/reg.h"
#include "idt.h"
#include "cpu/segment.h"
#include "stack.h"
#include "ui/ui.h"

extern jmp_buf jbuf;
extern uint32_t main_entry;

void raise_intr(uint8_t NO)
{
	/* Get gate descriptor */
	//if (NO == 0x20) return;
	lnaddr_t idt_addr = cpu.idtr.base;
	size_t size = sizeof(GateDesc);
	GateDesc desc;
	*(uint32_t *)(&desc) = hwaddr_read(idt_addr + NO * size, 4);
	*((uint32_t *)(&desc) + 1) = hwaddr_read(idt_addr + NO * size + 4, 4);

	if ( !desc.present ) {
		return;
	}

	/* Store current infomation */
	push(cpu.eflags);
	push(cpu.cs);
	push(cpu.eip);
	
	Log("the eip stored is %x", cpu.eip);
	nemu_state = TEST_INT;

	/* Jump */
	cpu.cs = desc.segment;
	cpu.eip = (desc.offset_31_16 << 16) | (desc.offset_15_0);

	/* Debug */
	Log("NO %x", NO);
	Log("desc\n%08x\n%08x", 
			*(uint32_t *)(&desc), *((uint32_t *)(&desc) + 1));
	Log("new cs %x", cpu.cs);
	Log("new eip %x", cpu.eip);

	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}

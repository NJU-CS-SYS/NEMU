/* The processor locates the GDT and the current LDT in memory by
 * means of the GDTR and LDTR registers.
 * These registers store the base address of the tables in the
 * linear address space and store the segment limits.
 *
 * The instructions LGDT and SGDT give access to the GDTR;
 */
#include "exec/helper.h"
#include "exec/template-start.h"

/* LGDT -- Load Global Descriptor Table Resigter
 * 0F 01 /2 imm16 imm32
 * LGDT.Limit:Base <- imm16:24/32
 */

make_helper(concat(lgdt_, SUFFIX))
{
	uint16_t limit = instr_fetch(eip + 1, 2);
	uint32_t base = instr_fetch(eip + 2, 4);
	
	cpu.gdtr.limit = limit;
#if DATA_BYTE == 2
	cpu.gdtr.base = base & 0xffffff;
#else
	cpu.gdtr.base = base;
#endif

	return 1 + 2 + 4;
}
#include "exec/template-end.h"

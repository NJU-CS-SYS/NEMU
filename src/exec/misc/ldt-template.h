/* The processor locates the GDT and the current LDT in memory by
 * means of the GDTR and LDTR registers.
 * These registers store the base address of the tables in the
 * linear address space and store the segment limits.
 *
 * The instructions LGDT and SGDT give access to the GDTR;
 */
#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

/* LGDT -- Load Global Descriptor Table Resigter
 * 0F 01 /2 imm16&24/imm16&32
 * LGDT.Limit:Base <- imm16:24/32
 * imm16&32 means an memory address with a data pair
 */

make_helper(concat(lgdt_, SUFFIX))
{
	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);
	test(m.reg == 2, "wrong reg domain");
	test(m.mod != 3, "wrong mod domain, expected addressing?");
	swaddr_t addr;
	int len = read_ModR_M(eip + 2, &addr);
	uint16_t limit = swaddr_read(addr, 2);
	uint32_t base = swaddr_read(addr + 2, 4);
	cpu.gdtr.limit = limit;
#if DATA_BYTE == 2
	cpu.gdtr.base = base & 0xffffff;
#else
	cpu.gdtr.base = base;
#endif
	print_asm("lgdt" str(SUFFIX) " %#x", addr);
	return 1 + 1 + len;
}
make_helper(concat(lidt_, SUFFIX))
{
	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);
	test(m.reg == 3, "wrong reg domain");
	test(m.mod != 3, "wrong mod domain, expected addressing");
	
	swaddr_t addr;
	int len = read_ModR_M(eip + 2, &addr);
	cpu.idtr.limit = swaddr_read(addr, 2);
#if DATA_BYTE == 2
	cpu.idtr.base = swaddr_read(addr + 2, 4) & 0xffffff;
#else
	cpu.idtr.base = swaddr_read(addr + 2, 4);
#endif

	print_asm("lidt" str(SUFFIX) " %#x", addr);
	return 1 + 1 + len;
}
#include "exec/template-end.h"

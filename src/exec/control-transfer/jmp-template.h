#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

/* The JMP ptr16:16 and ptr16:32 forms of the instructions use a four-byte
 * or six-byte operand as a long pointer to the destination.
 * The JMP and forms fetch the long pointer from the memory location 
 * specified(indirection). In Real Address Mode or Virtual 8086 Mode, the
 * long pointer provides 16 bits for the CS register and 16 or 32 bits for
 * the EIP register. In Protected Mode, both long pointer foms consult the 
 * Access Rights(AR) byte in the descriptor indexed by the selector part of
 * the long pointer
 */

/* Jump intersegment, 6-byte immediate address
 * ea ptr16:32 (note little endian : 32:16 in reality)
 * ljmp cs:eip
 */
#if DATA_BYTE == 4
make_helper(ljmp)
{
	/* TODO Protecion Model Access Right Check */
	uint32_t new_eip = instr_fetch(eip + 1, 4);
	uint16_t new_cs = instr_fetch(eip + 5, 2);
	cpu.eip = new_eip - 7;
	cpu.cs = new_cs;
	print_asm("ljmp $%#x,$%#x", new_cs, new_eip);
	return 1 + 4 + 2;
}
#endif
#include "exec/template-end.h"

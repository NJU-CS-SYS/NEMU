#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "nemu.h"

/* this head file deals with instructions
 * such as push and pop which operate the stack,
 * and have the alike format when in 16 bits and
 * 32 bits situation 
 * NOTE: not all push and pop can aply this template,
 * remember to implement them!
 */

/* 50 +rb PUSH r16  PUSH register word
 * 50 +rw PUSH r32  PUSH register dword
 */
make_helper(concat(push_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	/* according to 80386 manual, push esp/sp should be
	 * the old value of esp/sp,
	 * so we minus esp first by not store its result.
	 * Due to little endian, esp should get smaller.
	 */
	Log("R_ESP is %x and DATA_BYTE is %d at EIP %x instr %d", REG(R_ESP), DATA_BYTE, eip, MEM_R(eip));
	MEM_W( REG(R_ESP) - DATA_BYTE, REG(reg_code) );
	REG(R_ESP) += DATA_BYTE;

	print_asm("push" str(SUFFIX) " %%%s", REG_NAME(reg_code));
	return 1;
}

#include "exec/template-end.h"

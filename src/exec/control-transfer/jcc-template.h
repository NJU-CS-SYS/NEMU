#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(je_, SUFFIX)) {
	DATA_TYPE disp = instr_fetch(eip + 1, DATA_BYTE);
	if (FLAG_VAL(ZF)) cpu.eip += (int)disp; /* sign extended */
	if (DATA_BYTE == 2) cpu.eip &= 0x0000ffff;
	print_asm("je %x,", cpu.eip + DATA_BYTE + 1);
	return DATA_BYTE + 1;
}
#include "exec/template-end.h"

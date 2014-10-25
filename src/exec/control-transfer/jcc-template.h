#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(je_, SUFFIX)) {
	DATA_TYPE disp = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("je $0x%x,", disp);
	if (FLAG_VAL(ZF)) cpu.eip += (int)disp; /* sign extended */
	if (suffix == 'w') cpu.eip &= 0x0000ffff;
	return DATA_BYTE + 1;
}
#include "exec/template-end.h"

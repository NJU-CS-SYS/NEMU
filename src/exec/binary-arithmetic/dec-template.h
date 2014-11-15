#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"

#define DEC_FLAG(dest, result)\
	do {\
		FLAG_CHG(OF,OVERFLOW(1, dest, result));\
		FLAG_CHG(SF, MSB(result));\
		FLAG_CHG(ZF, result==0);\
		FLAG_CHG(AF, ADJUST(1, dest));\
		FLAG_CHG(PF, PARITY(result));\
	} while (0)

make_helper(concat(dec_rm_, SUFFIX)) {
	DATA_TYPE dest, result;
	int len = 1;
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	test(m.reg == 0, "wrong dispatch");

	if (m.mod == 3) {
		dest = REG(m.R_M);
		REG(m.R_M) = dest - 1;
		print_asm("dec %%%s", REG_NAME(m.R_M));
		len++;
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 1, &addr);
		dest = MEM_R(addr);
		MEM_W(addr, dest = 1);
		print_asm("dec %s", ModR_M_asm);
	}	
	result = dest - 1;
	DEC_FLAG(dest, result);
	return len;
}

make_helper(concat(dec_r_, SUFFIX)) {
	DATA_TYPE dest, result;
	int regcode = instr_fetch(eip, 1) & 0x7;
	dest = REG(regcode);
	result = dest - 1;
	DEC_FLAG(dest, result);
	REG(regcode) = result;
	print_asm("dec %%%s", REG_NAME(regcode));
	return 1;
}
#include "exec/template-end.h"

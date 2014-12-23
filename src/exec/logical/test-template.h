#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"

#define TEST_FLAG(x) do{\
	FLAG_CHG(CF, 0);\
	FLAG_CHG(OF, 0);\
	FLAG_CHG(SF, MSB(x));\
	FLAG_CHG(ZF, x==0);\
	FLAG_CHG(PF, PARITY(x));\
}while(0)

make_helper(concat(test_r_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE AND = REG(m.reg);
	uint32_t len = 1;
	if(m.mod == 3) {
		AND &= REG(m.R_M);
		print_asm("test" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len += read_ModR_M(eip + 1, &addr);
		AND &= MEM_R(addr);
		print_asm("test" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}
	TEST_FLAG(AND);
	return len + 1;
}

make_helper(concat(test_i2r_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE buf = REG(R_EAX) & imm;
	TEST_FLAG(buf);
	print_asm("test" str(SUFFIX) " $0x%x", imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(test_i2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_I2RM(test, DATA_BYTE);
	result = src & dest;
	TEST_FLAG(result);
	return len;
}
#include "exec/template-end.h"

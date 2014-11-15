#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "../template.h"

#define OR_FLAG(result)\
	do {\
		FLAG_CHG(OF, 0);\
		FLAG_CHG(CF, 0);\
		FLAG_CHG(SF, MSB(result));\
		FLAG_CHG(ZF, result == 0);\
		FLAG_CHG(PF, PARITY(result));\
	} while (0)

#define OR(src, dest, result)\
	do {\
		result = src | dest;\
		OR_FLAG(result);\
	} while (0)

make_helper(concat(or_r2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_R2RM(or);

	OR(src, dest, result);

	if (addr) MEM_W(addr, result);
	else REG(m.R_M) = result;

	return len;
}

make_helper(concat(or_i82rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(or, 1);

	OR(src, dest, result);

	if (addr) MEM_W(addr, result);
	else REG(m.R_M) = result;

	return len;
} 

make_helper(concat(or_i2r_, SUFFIX)) {
	TEMP_VALUES_S;
	src = instr_fetch(eip + 1, DATA_BYTE);
	dest = REG(R_EAX); // al, ax, eax

	OR(src, dest, result);

	REG(R_EAX) = result;
	len = 1 + DATA_BYTE;
	print_asm("or" str(SUFFIX) " $0x%x,%%%s", src,REG_NAME(R_EAX));
	return len;
}

make_helper(concat(or_i2rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	test(m.reg == 1, "wrong dispatching in 83, or");
	TEMP_I2RM(or, DATA_BYTE);

	OR(src, dest, result);

	TEMP_RESULT2RM(result);
	return len;
}

make_helper(concat(or_rm2r_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_RM2R(or);

	OR(src, dest, result);

	REG(m.reg) = result;
	return len;
}

#include "exec/template-end.h"

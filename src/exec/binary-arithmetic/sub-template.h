#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(sub_i2r_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	int reg_code;
	switch (DATA_BYTE) {
		case 1: reg_code = R_AL; break;
		case 2: reg_code = R_AX; break;
		case 4: reg_code = R_EAX; break;
		default: reg_code = R_EAX; assert(0);
	}
	DATA_TYPE dest = REG(reg_code);
	DATA_TYPE result = dest;
	TEMP_SUB_I(imm, dest, result);
	REG(reg_code) = result;

	print_asm("sub" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(reg_code));
	return DATA_BYTE + 1;
}

make_helper(concat(sub_i2rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sub, DATA_BYTE);
	TEMP_SUB_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}

make_helper(concat(sub_i82rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sub, 1);
	TEMP_SUB_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}

make_helper(concat(sub_r2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_R2RM(sub);
	TEMP_SUB_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}
#include "exec/template-end.h"

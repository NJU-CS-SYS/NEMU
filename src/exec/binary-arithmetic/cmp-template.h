#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"
#include "ui/ui.h"

make_helper(concat(cmp_i8_rm_, SUFFIX))
{
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_I2RM(cmp, 1);
	if (src == 0xff || src == 0xffff || src == 0xffffffff)
	{
		printf("eip %#x: imm %x\n", eip, src);
		nemu_state = TEST_INT;
	}
	TEMP_SUB_I(src, dest ,result);
	return len;
}

make_helper(concat(cmp_r2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	src = REG(m.reg);
	TEMP_R2RM(cmp);
	TEMP_SUB_I(src, dest, result);
	return len;
}

make_helper(concat(cmp_rm2r_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	src = REG(m.reg);
	TEMP_RM2R(cmp);
	TEMP_SUB_I(dest, src, result);
	return len;
}

make_helper(concat(cmp_i2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_I2RM(cmp, DATA_BYTE);
	test(sizeof(src) == DATA_BYTE, "Unexpected size!");
	TEMP_SUB_I(src, dest, result);
	return len;
}

make_helper(concat(cmp_i2r_, SUFFIX)) {
	TEMP_VALUES;
	len = 1;
	src = instr_fetch(eip + 1, DATA_BYTE);
	len += DATA_BYTE;
	dest = REG(R_EAX);
	TEMP_SUB_I(src, dest, result);
	print_asm("cmp" str(SUFFIX) " $%#x,%s", src, REG_NAME(R_EAX));
	return len;
}
#include "exec/template-end.h"

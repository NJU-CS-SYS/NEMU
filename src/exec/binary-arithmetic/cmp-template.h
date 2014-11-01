#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(cmp_i8_rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(cmp, 1);
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

make_helper(concat(cmp_i2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_I2RM(cmp, DATA_BYTE);
	test(sizeof(src) == DATA_BYTE, "Unexpected size!");
	TEMP_SUB_I(src, dest, result);
	return len;
}

#include "exec/template-end.h"

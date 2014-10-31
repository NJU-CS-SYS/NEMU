#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(add_r2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_R2RM(add);

	TEMP_ADD_I(src, dest, result);

	if (addr) MEM_W(addr, result);
	else REG(m.R_M) = result;

//	Log("src = %d, dest = %d, result = %d", src, dest, result);
	return len;
}

make_helper(concat(add_i82rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(add, 1);

	TEMP_ADD_I(src, dest, result);

	if (addr) MEM_W(addr, result);
	else REG(m.R_M) = result;

//	Log("src = %d, dest = %d, result = %d", src, dest, result);
	return len;
}

#include "exec/template-end.h"

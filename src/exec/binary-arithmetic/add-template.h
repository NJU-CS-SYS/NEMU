/* Module: add
 * State : Finished
 * Data  : 2014/11/02 20:30
 */
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

make_helper(concat(add_i2r_, SUFFIX)) {
	TEMP_VALUES_S;
	src = instr_fetch(eip + 1, 1);
	dest = REG(R_EAX); // al, ax, eax
	TEMP_ADD_I(src, dest, result);
	REG(R_EAX) = result;
	len = 1 + DATA_BYTE;
	return len;
}

make_helper(concat(add_i2rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	test(m.reg == 0, "wrong dispatching in 83, add");
	TEMP_I2RM(add, DATA_BYTE);
	TEMP_ADD_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}
#include "exec/template-end.h"

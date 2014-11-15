#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"

make_helper(concat(adc_r2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_R2RM(adc);

	TEMP_ADC_I(src, dest, result);

	if (addr) MEM_W(addr, result);
	else REG(m.R_M) = result;

	return len;
}

make_helper(concat(adc_i82rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(adc, 1);

	TEMP_ADC_I(src, dest, result);

	if (addr) MEM_W(addr, result);
	else REG(m.R_M) = result;

	return len;
} 

make_helper(concat(adc_i2r_, SUFFIX)) {
	TEMP_VALUES_S;
	src = instr_fetch(eip + 1, DATA_BYTE);
	dest = REG(R_EAX); // al, ax, eax

	TEMP_ADC_I(src, dest, result);
	REG(R_EAX) = result;
	len = 1 + DATA_BYTE;
	print_asm("adc" str(SUFFIX) " $0x%x,%%%s", src,REG_NAME(R_EAX));
	return len;
}

make_helper(concat(adc_i2rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	test(m.reg == 2, "wrong dispatching in 83, adc");
	TEMP_I2RM(adc, DATA_BYTE);

	TEMP_ADC_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}

make_helper(concat(adc_rm2r_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_RM2R(adc);
	TEMP_ADC_I(dest, src, result);
	REG(m.reg) = result;
	return len;
}

#include "exec/template-end.h"

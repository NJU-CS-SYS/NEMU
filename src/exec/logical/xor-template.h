#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "../template.h"

make_helper(concat(xor_i2r_, SUFFIX)) {
	TEMP_VALUES;
	src = instr_fetch(eip + 1, DATA_BYTE);
	dest = REG(R_EAX);
	result = dest ^ src;
	REG(R_EAX) = result;
	print_asm("xor" str(SUFFIX) " $0x%x,%%%s", src, REG_NAME(R_EAX));
	len = 1 + DATA_BYTE;
	return len;
}

#include "exec/template-end.h"

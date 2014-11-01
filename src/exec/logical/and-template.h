#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(and_i82rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(and, 1);
	result = dest & src;
	AND_FLAG(src, dest, result);
	return len;
}

#include "exec/template-end.h"

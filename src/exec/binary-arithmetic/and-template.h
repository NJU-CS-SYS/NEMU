#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(and_i82rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I82RM(and);
	
	result = dest & src;

	AND_FLAG(src, dest, result);
	return len;
}

#include "exec/template-end.h"

#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(and_i82rm_, SUFFIX)) {
	DATA_TYPE dest, result;
	DATA_TYPE_S src;
	int len = 1;
	TEMP_MOD_RM;
	TEMP_I82RM(and);
	Log("src %x", (uint32_t)src);
	
	result = dest & src;

	AND_FLAG(src, dest, result);
	return len;
}

#include "exec/template-end.h"

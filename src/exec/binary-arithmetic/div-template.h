#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "../tempalte.h"

make_helper(concat(div_i2imp_, SUFFIX)) {
	uint64_t src, dst ,quo, rem;
	int len = 1;
	ModR_M m;
	MOD_RM2R(div, src, len);

#include "exec/template-end.h"

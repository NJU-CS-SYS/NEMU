#include "exec/helper.h"

#define DATA_BYTE 1
#include "mul-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mul-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mul-template.h"
#undef DATA_BYTE

extern char suffix;
v_helper(mul_rm2r);
v_helper(imul_rm2imp);
v_helper(imul_i8rm2r);
v_helper(imul_rm2r);
v_helper(imul_irm2r);

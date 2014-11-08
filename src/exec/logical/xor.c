#include "exec/helper.h"
#include "../template.h"

#define DATA_BYTE 1
#include "xor-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "xor-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "xor-template.h"
#undef DATA_BYTE

extern char suffix;
v_helper(xor_i2r);
v_helper(xor_i2rm);
v_helper(xor_r2rm);
v_helper(xor_rm2r);

#include "exec/helper.h"

#define DATA_BYTE 1
#include "shift-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "shift-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "shift-template.h"
#undef DATA_BYTE

extern char suffix;
v_helper(sal_i8);
v_helper(sar_i8);
v_helper(sal_12rm);
v_helper(sar_12rm);

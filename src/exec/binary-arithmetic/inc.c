#include "exec/helper.h"

#define DATA_BYTE 1
#include "inc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "inc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "inc-template.h"
#undef DATA_BYTE

extern char suffix;
v_helper(inc_r);
v_helper(inc_rm);

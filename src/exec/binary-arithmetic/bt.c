#include "exec/helper.h"

#define DATA_BYTE 2
#include "bt-template.h"
#undef DATA_BYTE
#define DATA_BYTE 4
#include "bt-template.h"
#undef DATA_BYTE

extern char suffix;

v_helper(bt_r2rm);
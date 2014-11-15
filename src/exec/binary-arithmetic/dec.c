#include "exec/helper.h"

#define DATA_BYTE 1
#include "dec-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "dec-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "dec-template.h"
#undef DATA_BYTE

extern char suffix;
v_helper(dec_r);
v_helper(dec_rm);

#include "exec/helper.h"

#define DATA_BYTE 1
#include "io-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "io-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "io-template.h"
#undef DATA_BYTE

extern char suffix;

v_helper(in_i2a);
v_helper(in_d2a);

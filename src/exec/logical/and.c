#include "exec/helper.h"
#define DATA_BYTE 1
#include "and-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "and-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "and-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(and_i82rm_v) {
    return (suffix == 'l' ? and_i82rm_l(eip) : and_i82rm_w(eip));
}

v_helper(and_i2r);
v_helper(and_i2rm);
v_helper(and_r2rm);
v_helper(and_rm2r);

#include "exec/helper.h"

#define DATA_BYTE 1
#include "add-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "add-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "add-template.h"
#undef DATA_BYTE

extern char suffix;
make_helper(add_r2rm_v) {
    return (suffix == 'l' ? add_r2rm_l(eip) : add_r2rm_w(eip));
}

make_helper(add_i82rm_v) {
    return (suffix == 'l' ? add_i82rm_l(eip) : add_i82rm_w(eip));
}

v_helper(add_i2r);
v_helper(add_i2rm);
v_helper(add_rm2r);

#include "exec/helper.h"

#define DATA_BYTE 1
#include "sbb-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "sbb-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sbb-template.h"
#undef DATA_BYTE


/* for instruction encoding overloading */

extern char suffix;

make_helper(sbb_i2r_v) {
    return (suffix == 'l' ? sbb_i2r_l(eip) : sbb_i2r_w(eip));
}
make_helper(sbb_i2rm_v) {
    return (suffix == 'l' ? sbb_i2rm_l(eip) : sbb_i2rm_w(eip));
}
make_helper(sbb_i82rm_v) {
    return (suffix == 'l' ? sbb_i82rm_l(eip) : sbb_i82rm_w(eip));
}
v_helper(sbb_r2rm);
v_helper(sbb_rm2r);

#include "exec/helper.h"

#define DATA_BYTE 1
#include "sub-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "sub-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sub-template.h"
#undef DATA_BYTE


/* for instruction encoding overloading */

extern char suffix;

make_helper(sub_i2r_v) {
    return (suffix == 'l' ? sub_i2r_l(eip) : sub_i2r_w(eip));
}
make_helper(sub_i2rm_v) {
    return (suffix == 'l' ? sub_i2rm_l(eip) : sub_i2rm_w(eip));
}
make_helper(sub_i82rm_v) {
    return (suffix == 'l' ? sub_i82rm_l(eip) : sub_i82rm_w(eip));
}
v_helper(sub_r2rm);
v_helper(sub_rm2r);
v_helper(neg_rm);

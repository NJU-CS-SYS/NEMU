#include "exec/helper.h"

#define DATA_BYTE 1
#include "or-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "or-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "or-template.h"
#undef DATA_BYTE

extern char suffix;
make_helper(or_r2rm_v) {
	return (suffix == 'l' ? or_r2rm_l(eip) : or_r2rm_w(eip));
}

make_helper(or_i82rm_v) {
	return (suffix == 'l' ? or_i82rm_l(eip) : or_i82rm_w(eip));
}

v_helper(or_i2r);
v_helper(or_i2rm);
v_helper(or_rm2r);

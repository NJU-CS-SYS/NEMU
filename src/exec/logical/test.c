#include "exec/helper.h"

#define DATA_BYTE 1
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "test-template.h"
#undef DATA_BYTE


/* for instruction encoding overloading */

extern char suffix;

make_helper(test_r_rm_v) {
	return (suffix == 'l' ? test_r_rm_l(eip) : test_r_rm_w(eip));
}

v_helper(test_i2r);
v_helper(test_i2rm);

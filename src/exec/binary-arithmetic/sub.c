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
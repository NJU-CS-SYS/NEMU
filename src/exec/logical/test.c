#include "exec/helper.h"

bool parity(uint32_t val) {
	val = 0xff & val;
	int c = 0;
	for (; val != 0; val = val >> 1)
		if (val & 0x1) c++;
	return !(c & 0x1);
}

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

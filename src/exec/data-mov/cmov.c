#include "exec/helper.h"

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(cmovg_v) {
	return ('l' == suffix ? cmovg_l(eip) : cmovg_w(eip));
}

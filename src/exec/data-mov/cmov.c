#include "exec/helper.h"

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE

extern char suffix;
#define make_cmov_v(x)\
	do{\
		return ('l' == suffix ? concat(concat(cmov,x),_l)(eip) : concat(concat(cmov,x),_w)(eip));\
	}while(0)

make_helper(cmovg_v) {
	return ('l' == suffix ? cmovg_l(eip) : cmovg_w(eip));
}
make_helper(cmovge_v) { make_cmov_v(ge); }

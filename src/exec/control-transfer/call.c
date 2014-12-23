#include "exec/helper.h"

#define DATA_BYTE 1
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "call-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(call_rel_v) {
	return (suffix == 'l' ? call_rel_l(eip) : call_rel_w(eip));
}

make_helper(leave_v) {
	return (suffix == 'l' ? leave_l(eip) : leave_w(eip));
}

make_helper(ret_near_v) {
	return (suffix == 'l' ? ret_near_l(eip) : ret_near_w(eip));
}
make_helper(iret_v)
{
	return ('l' == suffix ? iret_l(eip) : iret_w(eip));
}
v_helper(call_rm);
v_helper(ret_imm16);

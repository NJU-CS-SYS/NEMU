#ifndef __CALL_H__
#define __CALL_H__

make_helper(call_rel_w);
make_helper(call_rel_l);

make_helper(leave_w);
make_helper(leave_l);
make_helper(ret_near_w);
make_helper(ret_near_l);

make_helper(call_rel_v);
make_helper(leave_v);
make_helper(ret_near_v);

make_helper(iret_w);
make_helper(iret_l);
make_helper(iret_v);

all_helper(call_rm);
all_helper(ret_imm16);
#endif

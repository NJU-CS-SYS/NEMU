#ifndef __STACK_H__
#define __STACK_H__

make_helper(push_v);
all_helper(pop_stack2r);
all_helper(pop_stack2m);

make_helper(push_i2s_b);
make_helper(push_i2s_w);
make_helper(push_i2s_l);
make_helper(push_i2s_v);

make_helper(push_m2s_b);
make_helper(push_m2s_w);
make_helper(push_m2s_l);
make_helper(push_m2s_v);

make_helper(pusha_w);
make_helper(pusha_l);
make_helper(pusha_v);

make_helper(popa_w);
make_helper(popa_l);
make_helper(popa_v);
#endif

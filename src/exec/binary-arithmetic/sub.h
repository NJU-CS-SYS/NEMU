#ifndef __SUB_H__
#define __SUB_H__

make_helper(sub_i2r_b);
make_helper(sub_i2r_w);
make_helper(sub_i2r_l);
make_helper(sub_i2rm_b);
make_helper(sub_i2rm_w);
make_helper(sub_i2rm_l);
make_helper(sub_i82rm_w);
make_helper(sub_i82rm_l);

make_helper(sub_i2r_v);
make_helper(sub_i2rm_v);
make_helper(sub_i82rm_v);

all_helper(sub_r2rm);
all_helper(sub_rm2r);
all_helper(neg_rm);
#endif

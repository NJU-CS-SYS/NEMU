#ifndef __SBB_H__
#define __SBB_H__

make_helper(sbb_i2r_b);
make_helper(sbb_i2r_w);
make_helper(sbb_i2r_l);
make_helper(sbb_i2rm_b);
make_helper(sbb_i2rm_w);
make_helper(sbb_i2rm_l);
make_helper(sbb_i82rm_w);
make_helper(sbb_i82rm_l);

make_helper(sbb_i2r_v);
make_helper(sbb_i2rm_v);
make_helper(sbb_i82rm_v);

all_helper(sbb_r2rm);
all_helper(sbb_rm2r);
#endif

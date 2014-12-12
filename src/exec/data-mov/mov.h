#ifndef __MOV_H__
#define __MOV_H__

make_helper(mov_i2r_b);
make_helper(mov_i2rm_b);
make_helper(mov_r2rm_b);
make_helper(mov_rm2r_b);
make_helper(mov_a2moffs_b);
make_helper(mov_moffs2a_b);

make_helper(mov_i2r_v);
make_helper(mov_i2rm_v);
make_helper(mov_r2rm_v);
make_helper(mov_rm2r_v);
make_helper(mov_a2moffs_v);
make_helper(mov_moffs2a_v);

make_helper(movzbw);
make_helper(movzbl);
make_helper(movzwl);
make_helper(movzbv);
all_helper(movs_b2r);
all_helper(movs_w2r);
all_helper(movs_m2m);

make_helper(mov_CR2r);
make_helper(mov_r2CR);
make_helper(mov_r2seg);

#endif

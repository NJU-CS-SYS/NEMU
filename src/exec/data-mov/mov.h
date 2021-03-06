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

make_helper(movz_b2_w);
make_helper(movz_b2_l);
make_helper(movz_w2_l);
make_helper(movz_b2_v);
all_helper(movs_b2r);
all_helper(movs_w2r);
all_helper(movs_m2m);

make_helper(mov_CR2r);
make_helper(mov_r2CR);
make_helper(mov_r2seg);

#endif

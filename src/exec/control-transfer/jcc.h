#ifndef __JCC_H__
#define __JCC_H__

make_helper(je_b);
make_helper(je_l);
make_helper(je_w);
make_helper(jmp_rel_b);
make_helper(jmp_rel_w);
make_helper(jbe_rel_b);
make_helper(jle_rel_b);

make_helper(je_v);
make_helper(jbe_rel_v);

#endif

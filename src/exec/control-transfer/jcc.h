#ifndef __JCC_H__
#define __JCC_H__

all_helper(jmp_rel);

all_helper(je);
all_helper(jne_rel);

all_helper(jb_rel);
all_helper(jbe_rel);
all_helper(jnb_rel);
all_helper(jnbe_rel);

all_helper(jl_rel);
all_helper(jle_rel);
all_helper(jnl_rel);
all_helper(jnel_rel);

all_helper(jg_rel);
all_helper(jge_rel);
all_helper(jng_rel);
all_helper(jnge_rel);

all_helper(ja_rel);
all_helper(jae_rel);
all_helper(jna_rel);
all_helper(jnae_rel);

all_helper(jcxz_rel);
all_helper(jecxz_rel);

all_helper(jno_rel);
all_helper(jnp_rel);
all_helper(jns_rel);
all_helper(jnz_rel);
all_helper(jo_rel);
all_helper(jp_rel);
all_helper(js_rel);

all_helper(jmp_rm);
#endif

#include "exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE


/* for instruction encoding overloading */

extern char suffix;

v_helper(je)
v_helper(jne_rel)
v_helper(jbe_rel)
v_helper(jl_rel)
v_helper(jle_rel)
v_helper(jg_rel)
v_helper(jge_rel)
v_helper(jmp_rel)
v_helper(jcxz_rel)
v_helper(jecxz_rel)
v_helper(ja_rel)
v_helper(jae_rel)
v_helper(jna_rel)
v_helper(jnae_rel)
v_helper(jnb_rel)
v_helper(jnbe_rel)
v_helper(jng_rel)
v_helper(jnge_rel)
v_helper(jnl_rel)
v_helper(jnel_rel)
v_helper(jno_rel)
v_helper(jnp_rel)
v_helper(jns_rel)
v_helper(jnz_rel)
v_helper(jo_rel)
v_helper(jp_rel)
v_helper(js_rel)

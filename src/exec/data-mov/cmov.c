#include "exec/helper.h"

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE

extern char suffix;
#define make_cmov_v(x)\
    do{\
        return ('l' == suffix ? concat(concat(cmov,x),_l)(eip) : concat(concat(cmov,x),_w)(eip));\
    }while(0)
make_helper(cmovae_v) { make_cmov_v(ae); }
make_helper(cmova_v) { make_cmov_v(a); }
make_helper(cmovbe_v) { make_cmov_v(be); }
make_helper(cmovb_v) { make_cmov_v(b); }
make_helper(cmovcxz_v) { make_cmov_v(cxz); }
make_helper(cmovecxz_v) { make_cmov_v(ecxz); }
make_helper(cmove_v) { make_cmov_v(e); }
make_helper(cmovge_v) { make_cmov_v(ge); }
make_helper(cmovg_v) { make_cmov_v(g); }
make_helper(cmovle_v) { make_cmov_v(le); }
make_helper(cmovl_v) { make_cmov_v(l); }
make_helper(cmovnae_v) { make_cmov_v(nae); }
make_helper(cmovna_v) { make_cmov_v(na); }
make_helper(cmovne_v) { make_cmov_v(ne); }
make_helper(cmovnge_v) { make_cmov_v(nge); }
make_helper(cmovng_v) { make_cmov_v(ng); }
make_helper(cmovnl_v) { make_cmov_v(nl); }
make_helper(cmovno_v) { make_cmov_v(no); }
make_helper(cmovnp_v) { make_cmov_v(np); }
make_helper(cmovns_v) { make_cmov_v(ns); }
make_helper(cmovnz_v) { make_cmov_v(nz); }
make_helper(cmovo_v) { make_cmov_v(o); }
make_helper(cmovp_v) { make_cmov_v(p); }
make_helper(cmovs_v) { make_cmov_v(s); }

#undef make_cmov_v

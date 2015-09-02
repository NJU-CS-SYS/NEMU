#ifndef __CMOV_H__
#define __CMOV_H__

#include "exec/helper.h"
#define make_cmov_h(x)\
    make_helper(concat(concat(cmov, x), _w));\
    make_helper(concat(concat(cmov, x), _l));\
    make_helper(concat(concat(cmov, x), _v));

make_cmov_h(ae)
make_cmov_h(a)
make_cmov_h(be)
make_cmov_h(b)
make_cmov_h(cxz)
make_cmov_h(ecxz)
make_cmov_h(e)
make_cmov_h(ge)
make_cmov_h(g)
make_cmov_h(le)
make_cmov_h(l)
make_cmov_h(nae)
make_cmov_h(na)
make_cmov_h(ne)
make_cmov_h(nge)
make_cmov_h(ng)
make_cmov_h(nl)
make_cmov_h(no)
make_cmov_h(np)
make_cmov_h(ns)
make_cmov_h(nz)
make_cmov_h(o)
make_cmov_h(p)
make_cmov_h(s)
#undef make_cmov_h
#endif

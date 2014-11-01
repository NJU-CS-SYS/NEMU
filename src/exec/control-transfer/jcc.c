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

v_helper(je);
make_helper(jbe_rel_v) {
	return (suffix == 'l' ? jbe_rel_l(eip) : jbe_rel_w(eip));
}
make_helper(jl_rel_v) {
	return (suffix == 'l' ? jl_rel_l(eip) : jl_rel_w(eip));
}

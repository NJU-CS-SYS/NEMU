#include "exec/helper.h"

extern char suffix;

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


make_helper(je_v) {
	return (suffix == 'l' ? je_l(eip) : je_w(eip));
}

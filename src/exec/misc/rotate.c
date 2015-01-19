#include "exec/helper.h"

#define DATA_BYTE 1
#include "rotate-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "rotate-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "rotate-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(rol_i82rm_v)
{
	return ('l' == suffix ? rol_i82rm_l(eip) : rol_i82rm_w(eip));
}

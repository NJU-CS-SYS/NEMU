#include "exec/helper.h"

#define DATA_BYTE 2
#include "ldt-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ldt-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(lgdt_v)
{
	Log("hello");
	return ('l' == suffix ? lgdt_l(eip) : lgdt_w(eip));
}
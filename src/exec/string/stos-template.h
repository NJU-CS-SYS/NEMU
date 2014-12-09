#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(stos_, SUFFIX)) // Store String Data
{
	Log("%02x", FLAG_VAL(DF));
	return 0;
}

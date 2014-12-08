#include "exec/helper.h"
#include "cpu/segment.h"

make_helper(exec);

make_helper(cs_override) // CS segment override prefix
{
	Sreg = CS;
	Log("CS segment override");
	return 1 + exec(eip + 1);
}

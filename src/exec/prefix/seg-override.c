#include "exec/helper.h"
#include "cpu/segment.h"

make_helper(exec);

make_helper(cs_override) // CS segment override prefix
{
    Sreg = CS;
    Log("CS segment override");
    return 1 + exec(eip + 1);
}

make_helper(ss_override) // CS segment override prefix
{
    Sreg = SS;
    Log("SS segment override");
    return 1 + exec(eip + 1);
}

make_helper(ds_override) // CS segment override prefix
{
    Sreg = DS;
    Log("CS segment override");
    return 1 + exec(eip + 1);
}

make_helper(es_override) // CS segment override prefix
{
    Sreg = ES;
    Log("ES segment override");
    return 1 + exec(eip + 1);
}

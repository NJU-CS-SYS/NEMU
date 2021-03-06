#include "exec/helper.h"

#define DATA_BYTE 1
#include "stack-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "stack-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "stack-template.h"
#undef DATA_BYTE


/* for instruction encoding overloading */

extern char suffix;

make_helper(push_v) {
    return (suffix == 'l' ? push_l(eip) : push_w(eip));
}
make_helper(push_i2s_v)
{
    return (suffix == 'l' ? push_i2s_l(eip) : push_i2s_w(eip));
}
make_helper(push_m2s_v)
{
    return (suffix == 'l' ? push_m2s_l(eip) : push_m2s_w(eip));
}
make_helper(pusha_v)
{
    return ('l' == suffix ? pusha_l(eip) : pusha_w(eip));
}
make_helper(popa_v)
{
    return ('l' == suffix ? popa_l(eip) : popa_w(eip));
}
v_helper(pop_stack2m);
v_helper(pop_stack2r);


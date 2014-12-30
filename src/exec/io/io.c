#include "exec/helper.h"

#define DATA_BYTE 1
#include "io-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "io-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "io-tempalte.h"
#undef DATA_BYTE

extern char suffix;

make_helper(io_i2a_v)
{ return ('l' == suffix ? io_i2a_l(eip) : io_i2a_w(eip)); }

#include "exec/helper.h"

#define DATA_BYTE 1
#include "add-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "add-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "add-template.h"
#undef DATA_BYTE

extern char suffix;
make_helper(add_r2rm_v) {
	return (suffix == 'l' ? add_r2rm_l(eip) : add_r2rm_w(eip));
}
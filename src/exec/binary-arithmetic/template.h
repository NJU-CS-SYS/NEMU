#include "exec/helper.h"
//#include "exec/flag.h"

#define TEMP_SUB_I(src, dest)\
if (sizeof(src) == 1 && sizeof(dest) > 1) {\
	dest = dest - (DATA_TYPE_S)src;\
} else {\
	dest = dest - src;\
}

#define TEMP_SUB_FLAG(src, dest, result)\
FLAG_CHG(OF,OVERFLOW(src, dest, result));\
FLAG_CHG(SF, MSB(result));\
FLAG_CHG(ZF, result==0);\
FLAG_CHG(AF, ADJUST(src, dest));\
FLAG_CHG(PF, PARITY(result));\
FLAG_CHG(CF, result < dest)

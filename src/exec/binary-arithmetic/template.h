#include "exec/helper.h"
//#include "exec/flag.h"

#define TEMP_SUB_I(src, dest)\
	if (sizeof(src) == 1 && sizeof(dest) == 2) {\
		dest = dest - (int16_t)src;\
	} else if (sizeof(src) == 1 && sizeof(dest) == 4) {\
		dest = dest - (int32_t)src;\
	} else {\
		dest = dest - src;\
	}

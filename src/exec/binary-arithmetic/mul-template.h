#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "../template.h"

#if DATA_BYTE == 1
#define MUL_FLAG do{\
	FLAG_CHG(OF, REG(R_AH) != 0);\
	FLAG_CHG(CF, REG(R_AH) != 0);\
}while(0)

#define MUL_RST(rst) do{\
	reg_w(R_AX) = rst;\
}while(0)
#elif DATA_BYTE == 2
#define MUL_FLAG do{\
	FLAG_CHG(OF, REG(R_DX) != 0);\
	FLAG_CHG(CF, REG(R_DX) != 0);\
}while(0)

#define MUL_RST(rst) do{\
	reg_w(R_DX) = (DATA_TYPE)(rst >> DATA_BYTE);\
	reg_w(R_AX) = (DATA_TYPE)(rst);\
}while(0)
#elif DATA_BYTE == 4
#define MUL_FLAG do{\
	FLAG_CHG(OF, REG(R_EDX) != 0);\
	FLAG_CHG(CF, REG(R_EDX) != 0);\
}while(0)

#define MUL_RST(rst) do {\
	reg_l(R_EDX) = (DATA_TYPE)(rst >> DATA_BYTE);\
	reg_l(R_EAX) = (DATA_TYPE)(rst);\
}while(0)
#endif


make_helper(concat(mul_rm2r_, SUFFIX)) {
	uint64_t src, dst, rst;
	int len = 1;
	ModR_M m;
	dst = REG(R_EAX);
	MOD_RM2R(mul, src, len);
	rst = src * dst;
	MUL_FLAG;
	MUL_RST(rst);
	return len;
}

#undef MUL_FLAG
#undef MUL_RST
#include "exec/template-end.h"

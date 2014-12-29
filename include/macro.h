#ifndef __MACRO_H__
#define __MACRO_H__
/* #x => "x" */
#define str_temp(x) #x
#define str(x) str_temp(x)

/* x ## y => xy */
#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)

#define Mask(x, mask) ((uint32_t) x & mask) == mask
#define _PARITY(x) \
	(Mask(x, 0x00000001)) +\
	(Mask(x, 0x00000010)) +\
	(Mask(x, 0x00000100)) +\
	(Mask(x, 0x00001000))
#define PARITY(x) !((_PARITY(x)) & 0x00000001)

#define OVERFLOW(sub, past_sub, result) \
	(!MSB(sub) && !MSB(past_sub) && MSB(result)) ||\
	(MSB(sub) && MSB(past_sub) && !MSB(result))

//FIXME this may be wrong
#define ADJUST(sub, past_sub) \
	(((uint32_t)past_sub & 0xf) - ((uint32_t)sub & 0xf) < 0) \
	|| (((uint32_t)past_sub & 0xf) + ((uint32_t)sub & 0xf) > 0)

#define PUSH(src_data)\
	do{\
		Sreg = SS;\
		MEM_W( REG(R_ESP) - DATA_BYTE, (src_data) );\
		REG(R_ESP) -= DATA_BYTE;\
	}while(0);


#define POP(x)\
	do{\
		Sreg = SS;\
		(x) = MEM_R(concat(reg_, SUFFIX)(R_ESP));\
		concat(reg_, SUFFIX)(R_ESP) += DATA_BYTE;\
	}while(0);

#endif

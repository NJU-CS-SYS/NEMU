#ifndef __SEGMENT_H__
#define __SEGMENT_H__
#include "common.h"

/* the 64bit segment descriptor */
typedef struct SegmentDescriptor {
	uint32_t limit_15_0          : 16;
	uint32_t base_15_0           : 16;
	uint32_t base_23_16          : 8;
	uint32_t type                : 4;
	uint32_t segment_type        : 1;
	uint32_t privilege_level     : 2;
	uint32_t present             : 1;
	uint32_t limit_19_16         : 4;
	uint32_t soft_use            : 1;
	uint32_t operation_size      : 1;
	uint32_t pad0                : 1;
	uint32_t granularity         : 1;
	uint32_t base_31_24          : 8;
} SegDesc;

lnaddr_t segment_translate(swaddr_t addr, size_t len);
hwaddr_t hwaddr_read(swaddr_t addr, size_t len);

extern int Sreg;
#endif

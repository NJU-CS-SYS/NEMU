#include "exec/helper.h"
#include "cpu/modrm.h"

#define OPCODE 0f_01
#include "0f-group-template.h"
#undef OPCODE

#define OPCODE 81
#include "group-template.h"
#undef OPCODE

#define OPCODE 80
#include "group-template.h"
#undef OPCODE

#define OPCODE 83
#include "group-template.h"
#undef OPCODE

#define OPCODE c0
#include "group-template.h"
#undef OPCODE

#define OPCODE c1
#include "group-template.h"
#undef OPCODE

#define OPCODE d0
#include "group-template.h"
#undef OPCODE

#define OPCODE d1
#include "group-template.h"
#undef OPCODE

#define OPCODE d2
#include "group-template.h"
#undef OPCODE

#define OPCODE d3
#include "group-template.h"
#undef OPCODE

#define OPCODE 8f
#include "group-template.h"
#undef OPCODE

#define OPCODE f6
#include "group-template.h"
#undef OPCODE

#define OPCODE f7
#include "group-template.h"
#undef OPCODE

#define OPCODE fe
#include "group-template.h"
#undef OPCODE

#define OPCODE ff
#include "group-template.h"
#undef OPCODE

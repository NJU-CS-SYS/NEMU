#include "exec/helper.h"
#include "cpu/modrm.h"

#define OPCODE 81
#include "group-template.h"
#undef OPCODE

#define OPCODE 80
#include "group-template.h"
#undef OPCODE

#define OPCODE 83
#include "group-template.h"
#undef OPCODE
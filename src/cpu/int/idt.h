typedef struct GateDescriptor {
    uint32_t offset_15_0      : 16;
    uint32_t segment          : 16;
    uint32_t pad0             : 8;
    uint32_t type             : 4;
    uint32_t system           : 1;
    uint32_t privilege_level  : 2;
    uint32_t present          : 1;
    uint32_t offset_31_16     : 16;
} GateDesc;

typedef struct TrapFrame {
    uint32_t edi, esi, ebp, xxx, ebx, edx, ecx, eax;    // GPRs
    int32_t irq;                                        // #irq
    uint32_t error_code;                                // error code
    uint32_t eip, cs, eflags;                            // execution state saved by hardware
} TrapFrame;

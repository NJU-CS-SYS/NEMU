#include "exec/helper.h"
#include "io/port-io.h"
#include "ui/ui.h"
#include "exec/template-start.h"
make_helper(concat(in_i2a_, SUFFIX))
{
    ioaddr_t ioaddr = instr_fetch(eip + 1, 1);
    REG(R_EAX) = pio_read(ioaddr, DATA_BYTE);

    print_asm("in $%#x", ioaddr);

    return 2;
}
make_helper(concat(in_d2a_, SUFFIX))
{
    ioaddr_t ioaddr = reg_l(R_DX);
    REG(R_EAX) = pio_read(ioaddr, DATA_BYTE);
    printf(str(SUFFIX) "eip %x: in port %x, data %x\n", eip, ioaddr, REG(R_EAX));
    print_asm("in %s,%%%s", "(%dx)", REG_NAME(R_EAX));

    return 1;
}
make_helper(concat(out_i2a_, SUFFIX))
{
    ioaddr_t ioaddr = instr_fetch(eip + 1, 1);
    pio_write( ioaddr, DATA_BYTE, REG(R_EAX) );

    print_asm("out %#x", ioaddr);
    return 2;
}
make_helper(concat(out_d2a_, SUFFIX))
{
    ioaddr_t ioaddr = reg_w(R_DX);
    pio_write( ioaddr, DATA_BYTE, REG(R_EAX) );
    printf("eip %x: out port %x, data %x\n", eip, ioaddr, REG(R_EAX));
    print_asm("out %%%s,%s", REG_NAME(R_EAX), "(%dx)");
    return 1;
}

#include "exec/template-end.h"

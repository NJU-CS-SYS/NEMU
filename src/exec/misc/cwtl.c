#include "exec/helper.h"
#include "cpu/reg.h"
#include "ui/ui.h"

make_helper(cwtl)
/* 
 * Sign-extended word to dword
 */
{
    int16_t word;
    int32_t dword;

    word = reg_w(R_AX);
    dword = word;        // Sign-extended
    reg_l(R_EAX) = dword;

    print_asm("cwtl");
    return 1;
}

make_helper(cltd)
/*
 * Sign-extended dword to qword
 */
{
    int32_t dword;
    int64_t qword;

    dword = reg_l(R_EAX);
    qword = dword;

    reg_l(R_EAX) = qword & (-1);
    reg_l(R_EDX) = (qword >> 32) & (-1);

    print_asm("cltd");
    return 1;
}

int main()
{
    asm volatile ( "movl $67, %eax;"    // system call ID, 67 = write_int
                   "movl $123, %ebx;"    // write this integer
                   "int $0x80");
    return 0;
}
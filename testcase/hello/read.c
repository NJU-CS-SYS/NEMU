char *prompt = "Enter an integer:";
int main()
{
    asm volatile ( "movl $66, %eax;"        // system call ID, 66 = READ_INT
                   "movl $prompt, %ebx;"    // prompt 
                   "int $0x80");
    return 0;
}

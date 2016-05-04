#include "ui/ui.h"
#include "ui/breakpoint.h"
#include "../memory/page.h"

#include "nemu.h"

#include <signal.h>
#include <stdlib.h>

extern uint64_t L1_hit;
extern uint64_t L2_hit;
extern uint64_t mem_access;
extern uint64_t L2_access;
extern uint64_t L1_access;
extern uint32_t main_entry;

int WATCH_EIP = 0;
int nemu_state = END;
int bp_state = INIT;
int wp_state = OFF;
swaddr_t bp_backup = 0;

void cpu_exec(uint32_t);
void restart();
void test_tokens(char* e);
uint32_t calculate(char* e);
void L1_print(swaddr_t addr);
void L2_print(swaddr_t addr);
uint32_t hwaddr_read(hwaddr_t,int);
char *read_func_name(swaddr_t addr, swaddr_t *value);

struct _frame_node {
    char *name;
    swaddr_t addr;
    struct _frame_node *next;
};
typedef struct _frame_node frame_node;
static char previous[64];

/* This function will be called when you press <C-c>. And it will return to 
 * where you press <C-c>. If you are interesting in how it works, please
 * search for "Unix signal" in the Internet.
 */

static void control_C(int signum)
{
    if(nemu_state == RUNNING) {
        nemu_state = INT;
    }
}
void init_signal()
{
    /* Register a signal handler. */
    struct sigaction s;
    /* sigaction is a system struct ! */
    memset(&s, 0, sizeof(s));
    s.sa_handler = control_C;
    int ret = sigaction(SIGINT, &s, NULL);
    assert(ret == 0);
}
static void cmd_c()
{
    if(nemu_state == END) {
        puts("The Program does not start. Use 'r' command to start the program.");
        return;
    }

    nemu_state = RUNNING;
    cpu_exec(-1);
    if(nemu_state != END) { nemu_state = STOP; }
}
static void cmd_r()
{
    if(nemu_state != END) { 
        char c;
        while(1) {
            printf("The program is already running. Restart the program? (y or n)");
            fflush(stdout);
            int ret = scanf("%c",&c);
            if (ret < 1) {
                printf("Input error, assume no\n");
                c = 'n';
            }
            switch(c) {
                case 'y': goto restart_;
                case 'n': return;
                default: puts("Please answer y or n.");
            }
        }
    }

restart_:
    restart();
    nemu_state = STOP; /* HIT good trap here !? */
    cmd_c();
}
static void cmd_si()
{
    char* p = strtok(NULL, " ");
    int run_times = 1;
    if (p != NULL) { run_times = atoi(p); }
    if (nemu_state == END) {
        restart();
    }
    nemu_state = RUNNING;
    cpu_exec(run_times);
    if(nemu_state != END) { nemu_state = STOP; }
}
static void cmd_info()
{
    char* opt = strtok(NULL, " ");
    if (opt == NULL || strcmp(opt, "r") == 0) {
        printf("%-15s%#-15X%u\n","eax", cpu.eax, cpu.eax);
        printf("%-15s%#-15X%u\n","ecx", cpu.ecx, cpu.ecx); 
        printf("%-15s%#-15X%u\n","edx", cpu.edx, cpu.edx); 
        printf("%-15s%#-15X%u\n","ebx", cpu.ebx, cpu.ebx); 
        printf("%-15s%#-15X%u\n","esp", cpu.esp, cpu.esp); 
        printf("%-15s%#-15X%u\n","ebp", cpu.ebp, cpu.ebp); 
        printf("%-15s%#-15X%u\n","esi", cpu.esi, cpu.esi); 
        printf("%-15s%#-15X%u\n","edi", cpu.edi, cpu.edi); 
        printf("%-15s%#-15X%u\n","eip", cpu.eip, cpu.eip); 
        printf("CF PF AF ZF SF TF IF DF OF\n"
               "%2d %2d %2d %2d %2d %2d %2d %2d %2d\n",
                FLAG_VAL(CF),
                FLAG_VAL(PF),
                FLAG_VAL(AF),
                FLAG_VAL(ZF),
                FLAG_VAL(SF),
                FLAG_VAL(TF),
                FLAG_VAL(IF),
                FLAG_VAL(DF),
                FLAG_VAL(OF)
              );
        printf("CS %x DS %x SS %x\n", cpu.cs, cpu.ds, cpu.ss);
    }
    else if (strcmp(opt, "b") == 0) {
        BP* current = getHead();
        printf("%-6s%-12s%-12s%s\n", "NUM", "TYPE", "ADDRESS", "EXPRESSION");
        while(current != NULL) {
            if (current->expr[0] == '\0') {
                printf("%-6d%-12s%#-12x%s\n", current->NO, "breakpoint", current->addr, current->expr);
            } else {
                printf("%-6d%-12s%#-12x%s\n", current->NO, "watchpoint", current->addr, current->expr);
            }
            current = current->next;
        }
    }
}
static void cmd_x()
{
    char* _num = strtok(NULL, " ");
    char* _addr = strtok(NULL, "");
    if((_num != NULL) && (_addr != NULL)) {
        int num = atoi(_num);
        swaddr_t addr = calculate(_addr);
        Log("rst addr is %x", addr);
        int i;
        for(i = 0; i < num; i++){
            int j;
            printf("%8x: ", addr+i*16);
            for(j = 0; j < 4; j++) {
                int k;
                for (k = 0; k < 4; k ++) {
                    printf("%02x", swaddr_read(addr+i*16+j*4+k, 1));
                }
                printf(" ");
            }
            putchar('\n');
        }
    }
}
static void cmd_s()
{
    char* _num = strtok(NULL, " ");
    char* _addr = strtok(NULL, "");
    if((_num != NULL) && (_addr != NULL)) {
        int num = atoi(_num);
        swaddr_t addr = calculate(_addr);
        Log("rst addr is %x", addr);
        int i;
        for(i = 0; i < num; i++){
            int j;
            printf("%8x: ", addr+i*16);
            for(j = 0; j < 4; j++) {
                int k;
                for (k = 0; k < 4; k ++) {
                    printf("%c", swaddr_read(addr+i*16+j*4+k, 1));
                }
                printf(" ");
            }
            putchar('\n');
        }
    }
}
static void cmd_b()
{
    char* p = strtok(NULL, "");
    if (p[0] == '*') {
        swaddr_t addr = calculate(p+1);
        add_bp(addr);
    }
}
static void cmd_d()
{
    char *opt = strtok(NULL, " ");
    if (opt == NULL) { 
        free_all(); 
        wp_state = OFF;
    }
    else {
        int no = atoi(opt);
        if (0 <= no && no < NR_BP) free_bp(no);
        else assert(0);
    }
}
static void cmd_e()
{
    char *expr = strtok(NULL, "");
    test_tokens(expr);
}
static void cmd_p()
{
    char *e = strtok(NULL, "");
    printf("%u\n", calculate(e));
}
static void cmd_w()
{
    char *expr = strtok(NULL, "");
    add_watchpoint(expr);
    wp_state = ON;
}
static void _put_bt (frame_node* cur, int i)
{
    if (cur == NULL) return;
    _put_bt(cur->next, i + 1);
    printf("#%d %-10s %x\n", i,
                (char*)cur->name,
                cur->addr);
}
static void cmd_bt()
{
    if (cpu.ebp == 0)
        return;

    swaddr_t eip = cpu.eip;
    uint32_t ebp = cpu.ebp;
    frame_node *head = (frame_node*)malloc(sizeof(frame_node));
    head->name = "head";
    head->addr = 0;
    head->next = NULL;
    frame_node *temp = NULL;
    do {
        temp = (frame_node *)malloc(sizeof(frame_node));
        temp->name = read_func_name(eip, &(temp->addr));
        temp->next = head->next;
        head->next = temp;
        Log("ebp %x", ebp);
        eip = swaddr_read(ebp + 4, 4);
        ebp = swaddr_read(ebp, 4);
    } while (ebp != 0x7fffffc);

    test(temp == head->next, "wrong loop end");

    _put_bt(head->next, 0);

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
static void cmd_l1()
{
    swaddr_t addr = strtol(strtok(NULL,""),NULL,16);
    L1_print(addr);
}
static void cmd_l2()
{
    swaddr_t addr = strtol(strtok(NULL,""),NULL,16);
    L2_print(addr);
}
static void cmd_check()
{
    printf("L1 access times : %llu\n", (long long unsigned int)L1_access);
    printf("L1 hit times    : %llu\n", (long long unsigned int)L1_hit);
    printf("L2 access times : %llu\n", (long long unsigned int)L2_access);
    printf("L2 hit times    : %llu\n", (long long unsigned int)L2_hit);
}

#if 0
static void print_table(uint32_t base, int indent)
{
    int i;
    PDE temp;
    for (i = 0; i < 4096; i = i + 4) {
        temp.val = hwaddr_read(base + i, 4);
        if (temp.present) {
            printf("%*s%05x #=> %05x\n", indent, "", base + i, temp.page_frame);
        }
    }
}
#endif
static void cmd_dir()
{
    int base = cpu.cr3.page_directory_base << 12;
    int i;
    char *p = strtok(NULL, "");
    if (p != NULL) base = strtoll(p, NULL, 16);
    printf("base %05x\n", base);
    for (i = 0; i < 4096; i += 4) {
        PDE temp;
        temp.val = hwaddr_read(base + i, 4);
        if (temp.present) {
            printf("%05x #=> %05x\n", base + i, temp.page_frame);
        }
    }
}
static void cmd_eip()
{
    char *p = strtok(NULL, "");
    WATCH_EIP = strtoll(p, NULL, 16);
}
union {
    struct {
        uint32_t page_offset : 12;
        uint32_t pte_idx : 10;
        uint32_t pde_idx : 10;
    };
    uint32_t addr;
} virtual_addr;

hwaddr_t page_translate(lnaddr_t, int);
static void cmd_pw()
{
    char *p = strtok(NULL, "");
    int addr = strtoll(p, NULL, 16);
    virtual_addr.addr = addr;
    printf("pde %d pte %d page %x\n", virtual_addr.pde_idx, virtual_addr.pte_idx, virtual_addr.page_offset);
    printf("%x #=> %x\n", addr, page_translate(addr, 4));
}

#ifdef SYS_LAB
void npc_gets(char buf[], size_t size);  // NPC memory-mapped stdin
#endif

void main_loop()
{
    char cmd[1024] = {};

    while(1) {
        printf("(nemu) ");
        fflush(stdout);
#ifdef SYS_LAB
        npc_gets(cmd, sizeof(cmd));
#else
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd) - 1] = '\0';
#endif

        char *p = strtok(cmd, " ");

        if (p == NULL) {
            p = previous;
        } else {
            strcpy(previous, p);
        }

        if(p == NULL) { continue; }

        if(strcmp(p, "q") == 0) { return; }
        else if(strcmp(p, "c") == 0) { cmd_c(); }
        else if(strcmp(p, "r") == 0) { cmd_r(); }
        else if(strcmp(p, "x") == 0) { cmd_x(); }
        else if(strcmp(p, "s") == 0) { cmd_s(); }
        else if(strcmp(p, "b") == 0) { cmd_b(); }
        else if(strcmp(p, "d") == 0) { cmd_d(); }
        else if(strcmp(p, "p") == 0) { cmd_p(); }
        else if(strcmp(p, "w") == 0) { cmd_w(); }
        else if(strcmp(p, "si") == 0) { cmd_si(); }
        else if(strcmp(p, "bt") == 0) { cmd_bt(); }
        else if(strcmp(p, "info") == 0) { cmd_info(); }
        else if(strcmp(p, "l1") == 0) { cmd_l1(); }
        else if(strcmp(p, "l2") == 0) { cmd_l2(); }
        else if(strcmp(p, "pw") == 0) { cmd_pw(); }
        else if(strcmp(p, "dir") == 0) { cmd_dir(); }
        else if(strcmp(p, "e") == 0) { cmd_e(); }
        else if(strcmp(p, "reload") == 0) { cpu.eip = 0x100000; }
        else if (strcmp(p, "check") == 0) { cmd_check(); }
        else if (strcmp(p, "eip") == 0) { cmd_eip(); }
        else { printf("Unknown command '%s'\n", p); }
    }
}

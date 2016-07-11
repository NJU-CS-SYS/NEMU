#include "nemu.h"
#include "ui/ui.h"
#include <stdlib.h>

int nemu_state = END;

void cpu_exec(uint32_t);
void restart();

#ifdef SYS_LAB
void npc_gets(char buf[], size_t size);  // NPC memory-mapped stdin
void npc_fputs(const char *s, FILE *fp);
void npc_fputs_chomp(const char *s, FILE *fp);
#endif

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
            int ret = getchar();  // Not using scanf to avoid symbol __iso99_scanf ...
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
}

static void cmd_x()
{
    char* _num = strtok(NULL, " ");
    char* _addr = strtok(NULL, "");
    if((_num != NULL) && (_addr != NULL)) {
        int num = atoi(_num);
        swaddr_t addr = strtol(_addr, NULL, 16);
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
        swaddr_t addr = strtol(_addr, NULL, 16);
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

void main_loop()
{
    char previous[64];
    char cmd[1024] = {};

    while(1) {
        //printf("(nemu) ");

#ifdef SYS_LAB
        npc_fputs_chomp("\n(nemu) ", NULL);
        npc_gets(cmd, sizeof(cmd));
        npc_fputs(cmd, NULL);
#else
        fflush(stdout);
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
        else if(strcmp(p, "si") == 0) { cmd_si(); }
        else if(strcmp(p, "info") == 0) { cmd_info(); }
        else { printf("Unknown command '%s'\n", p); }
    }
}

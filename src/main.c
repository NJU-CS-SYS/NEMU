#include "common.h"

#ifdef SYS_LAB
#include "monitor.h"
Monitor monitor;

#ifndef DEPLOY

int global_use_std = 1;  // 控制输出目标的开关，用于在模拟器初始化前只使用标准输出。
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>  // sigaction & etc.

void kb_callback(int unused);
void timer_callback(int unused);

#endif

#endif

void main_loop();
void init_device();
void init_sdl();
void set_exec_file(const char *filename);
void init_L1();

int enable_debug = false;
int quiet = false;

int main(int argc, char *argv[]) {

#ifdef SYS_LAB
#ifndef DEPLOY
    // 用于综合实验模拟内存映射式 I/O，本身涉及大量系统函数，交叉编译时必须去除。

    // 模拟中断处理函数
    struct sigaction sa = {};
    sa.sa_handler = kb_callback;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror(NULL);
    }
    sa.sa_handler = timer_callback;
    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror(NULL);
    }
    pid_t pid = getpid();  // 之后进行深拷贝，所以使用临时变量即可。
    init_monitor(&monitor, "monitor/config", &pid);
    global_use_std = 0;
#endif
#endif

    enable_debug = true;
    quiet = false;

    /* Initialization of device */
    init_device();

    main_loop();

    return 0;
}

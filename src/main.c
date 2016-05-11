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

/**
 * FIXME: 在这里临时定义系统原语，检查调用情况，尽可能删除。
 */
#ifdef DEPLOY
ssize_t write(int fd, const void *buf, size_t count) { return 0; }
ssize_t read(int fd, void *buf, size_t count) { return 0; }
void *sbrk(int *i) { return NULL; }
off_t lseek(int fd, off_t offset, int whence) { return 0; }
int fstat(int fd, void *buf) { return 0; }
int isatty(int fd) { return 0; }
int close(int fd) { return 0; }
#endif

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

    /* Perform some global initialization */
    init_L1();

    /* Initialization of device */
    init_device();

    main_loop();

    return 0;
}

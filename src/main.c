#include "common.h"

#ifdef SYS_LAB
#include "monitor.h"
int global_use_std = 1;  // 控制输出目标的开关，用于在模拟器初始化前只使用标准输出。
Monitor monitor;
#endif

void load_table();
void main_loop();
void init_device();
void init_sdl();
void set_exec_file(const char *filename);
void init_bp_pool();
void init_regex();
void init_signal();
void init_L1();

int enable_debug = false;
int quiet = false;

/**
 * FIXME: 在这里临时定义系统原语，检查调用情况，尽可能删除。
 */
#ifdef DEPLOY
ssize_t write(int fd, const void *buf, size_t count) { return 0; }
ssize_t read(int fd, void *buf, size_t count) { return 0; }
void *sbrk(void *i) { return NULL; }
off_t lseek(int fd, off_t offset, int whence) { return 0; }
int fstat(int fd, void *buf) { return 0; }
int isatty(int fd) { return 0; }
void close(int fd) {}
#endif

int main(int argc, char *argv[]) {

#ifdef SYS_LAB
#ifndef DEPLOY
    init_monitor(&monitor, "monitor/config");
    global_use_std = 0;
#endif
#endif

    set_exec_file(argv[1]);

    enable_debug = true;
    quiet = false;

    /* Perform some global initialization */
    init_regex();
    init_signal();
    init_bp_pool();
    load_table();
    init_L1();

    /* Initialization of device */
    init_device();
    init_sdl();

    main_loop();

    return 0;
}

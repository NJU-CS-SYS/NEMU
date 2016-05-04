#include "common.h"

#ifdef SYS_LAB
#include "monitor.h"
int global_use_std = 1;  // TODO cond-compile it
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
void reg_test();

int enable_debug = false;
int quiet = false;

int main(int argc, char *argv[]) {
#ifdef SYS_LAB
    init_monitor(&monitor, "monitor/config");  // TODO cond-compile it and get from commandline
    global_use_std = 0;  // TODO cond-compile it
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


    /* Test whether the 'CPU_state' structure is organized correctly. */
    reg_test();

    /* Initialization of device */
    init_device();
    init_sdl();

    main_loop();

    return 0;
}

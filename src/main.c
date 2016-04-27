#include "common.h"

#ifdef SYS_LAB
#include "monitor.h"
int global_use_std = 1;  // TODO cond-compile it
#endif

#include <unistd.h>

void load_table();
void main_loop();
void init_device();
void init_sdl();
void set_main_args(int, char * []);
void init_bp_pool();
void init_regex();
void init_signal();
void reg_test();

Monitor monitor;
int enable_debug = false;
int quiet = false;

static void process_args(int argc, char *argv[]) {
    int opt;
    while( (opt = getopt(argc, argv, "dq")) != -1) {
        switch(opt) {
            case 'd':
                enable_debug = true;
                break;
            case 'q':
                quiet = true;
                break;
            default:
                test(0, "bad option = %s\n", optarg);
                break;
        }
    }

    test(argc > optind, "Program is not given\n");

    set_main_args(argc - optind, argv + optind);
}

int main(int argc, char *argv[]) {
    init_monitor(&monitor, "monitor/config");  // TODO cond-compile it and get from commandline
    global_use_std = 0;  // TODO cond-compile it

    process_args(argc, argv);

    /* Perform some global initialization */
    init_regex();
    init_signal();
    init_bp_pool();
    load_table();


    /* Test whether the 'CPU_state' structure is organized correctly. */
    reg_test();

    /* Initialization of device */
    init_device();
    init_sdl();

    main_loop();

    return 0;
}

#include "common.h"
#include "sdl.h"
#include "ui/ui.h"

#include <sys/time.h>
#include <signal.h>

#define TIMER_HZ 100

extern void timer_intr();
extern void keyboard_intr();

static struct itimerval it = {};

static void device_update(int signum) {
    printf("time event!");
    timer_intr();

    // Ensure continuing timer
    int ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
    assert(ret == 0);
}

void init_sdl() {

    /*
     * Register a timer handler at a given interval
     */

    struct sigaction s = {};
    s.sa_handler = device_update;
    int ret = sigaction(SIGVTALRM, &s, NULL);
    assert(ret == 0);

    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 1000000 / TIMER_HZ;
    ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
    assert(ret == 0);
}

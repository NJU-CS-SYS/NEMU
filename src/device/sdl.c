#include "common.h"
#include "sdl.h"
#include "ui/ui.h"

#ifndef DEPLOY

#include <sys/time.h>
#include <signal.h>

#define TIMER_HZ 100

// TODO: 使用 timer_intr() 处理时间中断
// TODO: 使用 keyboard_intr() 处理键盘中断

static struct itimerval it = {};

static void device_update(int signum) {
    return;
    printf("time event!");
    timer_intr();

    // Ensure continuing timer
    int ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
    assert(ret == 0);
}

#endif

void init_sdl() {

#ifndef DEPLOY
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
#endif

}

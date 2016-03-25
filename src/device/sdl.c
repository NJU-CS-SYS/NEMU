#include "common.h"
#include "sdl.h"
#include "vga.h"
#include "ui/ui.h"
#include "keyboard.h"

#include <sys/time.h>
#include <signal.h>

extern uint8_t fontdata_8x16[128][16];
uint8_t (*pixel_buf) [SCREEN_COL];

#define TIMER_HZ 100

static uint64_t jiffy = 0;
static struct itimerval it;
extern void timer_intr();
extern void update_screen();

#define HAS_INTR(p) ( *(p) == 1 )
const static uint32_t *p_intr = (uint32_t *)0x9000; // where stores intr_signal 
const static uint32_t *p_ino = (uint32_t *)0x9999;  // where strores intr_no

static void device_update(int signum) {
    jiffy ++;
    timer_intr();
    if(jiffy % (TIMER_HZ / VGA_HZ) == 0) {
        update_screen();
    }

    if( HAS_INTR(p_intr) ) {
        // gte the interrupt number
        uint32_t intr_no = *p_ino;
        if( intr_no == KEYBOARD_IRQ ) {
            uint32_t code = get_scancode();
            // KEYDOWN
            if( key_type(code) == KEYDOWN ) {
                keyboard_intr( scancode2byte(code) );
            }
            // KEYUP
            else if( key_type(code) == KEYUP) {
                keyboard_intr( scancode2byte(code) | 0x80);
            }
        }
    }

    int ret = 0;
    ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
    assert(ret == 0);
}

void sdl_clear_event_queue() {
    SDL_Event event;
    while(SDL_PollEvent(&event));
}

void init_sdl() {
    //pixel_buf = screen->pixels;

    struct sigaction s;
    memset(&s, 0, sizeof(s));
    s.sa_handler = device_update;  
    ret = sigaction(SIGVTALRM, &s, NULL);
    assert(ret == 0);

    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 1000000 / TIMER_HZ;
    ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
    assert(ret == 0);
}
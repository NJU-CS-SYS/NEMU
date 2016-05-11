#include "common.h"
#include "vga.h"
#include "ui/ui.h"
#include "keyboard.h"

//#define TIMER_HZ 100

extern void timer_intr();

void device_update(int if_flag) {
    if(npc_keyboardintr() && if_flag) {
        // KEYDOWN
        char c = npc_getchar();
        keyboard_intr(c);
    }   
}

void clear_event_queue() {
    //*(p_intr) = INTR_NOT_COME;
}
#include "common.h"
#include "vga.h"
#include "ui/ui.h"
#include "keyboard.h"

#define INTR_COME 1
#define INTR_NOT_COME 0
#define HAS_INTR(p) ( *(p) == INTR_COME )
static uint32_t *p_intr = (uint32_t *)0x9000; // where stores intr_signal 
static uint32_t *p_ino = (uint32_t *)0x9999;  // where strores intr_no

#define VMEM_ADDR 0xA000
uint8_t *pixel_buf = (uint8_t*)(VMEM_ADDR);

extern uint8_t fontdata_8x16[128][16];

#define TIMER_HZ 100

static uint64_t jiffy = 0;
extern void timer_intr();
extern void update_screen();

void device_update(int if_flag) {
    jiffy ++;
    timer_intr();
    if(jiffy % (TIMER_HZ / VGA_HZ) == 0) {
        update_screen();
    }

    if( HAS_INTR(p_intr) && if_flag) {
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
                keyboard_intr( scancode2byte(code) | 0x80 );
            }
        }
    }    
}

void clear_event_queue() {
    *(p_intr) = INTR_NOT_COME;
}
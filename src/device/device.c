void init_serial();
void init_timer();
void init_vga();
void init_i8042();
void init_ide();

void init_device() {
    init_i8042();
    init_ide();
    init_vga();
}

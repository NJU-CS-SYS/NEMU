#ifndef MONITOR_H
#define MONITOR_H

// The monitor module mimics the behaviors of NPC's peripheral.
//   1). Memroy-mapped, random-accessible ascii text buffer, not serial output in stream;
//   2). memory-mapped keyboard state.

typedef struct {
    int ready;  // Indicate whether there is a key stroke, updated by monitor, read by nemu.
    char data;  // The key data, updated by monitor, read by nemu.
} KeyState;

typedef struct {
    int width;
    int height;
    KeyState *key_state;  // shared memory
    char *text_buffer;     // shared memory
} Monitor;

// Init the monitor structure from config file,
// and do memry mapping.
void monitor_init(Monitor *m, const char *config);

#endif // MONITOR_H

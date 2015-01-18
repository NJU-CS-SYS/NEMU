#include "hal.h"
#include "common.h"
#include "x86.h"

#define NR_KEYS 18

enum { KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS };

/* Only the following keys are used in NEMU-PAL. */
static const int keycode_array[] = {
	K_UP, K_DOWN, K_LEFT, K_RIGHT, K_ESCAPE,
	K_RETURN, K_SPACE, K_PAGEUP, K_PAGEDOWN, K_r,
	K_a, K_d, K_e, K_w, K_q,
	K_s, K_f, K_p
};
static const char* key_name[] = {
	"UP", "DOWN", "LEFT", "RIGHT", "ESC",
	"RET", "SPACE", "PAGE UP", "PAGE DOWN", "r",
	"a", "d", "e", "w", "q",
	"s", "f", "p"
};

static int key_state[NR_KEYS];
static int old_key = 0;

static inline int
get_keycode(int index) /* Return the keycode of key */
{
	assert(index >= 0 && index < NR_KEYS);
	return keycode_array[index];
}

static inline int
query_key(int index) /* Return the state of key */
{
	assert(index >= 0 && index < NR_KEYS);
	return key_state[index];
}

static inline void
release_key(int index)
{
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_WAIT_RELEASE;
}

static inline void
clear_key(int index)
{
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_EMPTY;
}

static inline void
press_key(int index)
{
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_PRESS;
}

static inline void
wait_key(int index)
{
	assert(index >= 0 && index < NR_KEYS);
	key_state[index] = KEY_STATE_WAIT_RELEASE;
}

void
keyboard_event(void) /* Handle type interrupt */
{
	/* Fetch the scancode and update the key states. */
	const int scan = in_byte(0x60);
	int i;
	// scan
	for (i = 0; i < NR_KEYS; i ++)
	{
		if (scan == get_keycode(i))
		{
			if (scan == old_key)
			{
				key_state[i] = KEY_STATE_WAIT_RELEASE;
			}
			else
			{
				key_state[i] = KEY_STATE_PRESS;
			}
		}
		else if (scan == keycode_array[i] + 0x80)
		{
			Log("Release %s", key_name[i]);
			key_state[i] = KEY_STATE_RELEASE;
		}
	}
	old_key = scan;
}

bool 
process_keys
(
 void (*key_press_callback)(int),
 void (*key_release_callback)(int)
)
{

	cli();

	/* Traverse the key states. Find a key just pressed or released.
	 * If a pressed key is found, call ``key_press_callback'' with the keycode.
	 * If a released key is found, call ``key_release_callback'' with the keycode.
	 * If any such key is found, the function return true.
	 * If no such key is found, the function return false.
	 * Remember to enable interrupts before returning from the function.
	 */

	int i;
	bool ret = false;

	for (i = 0; i < NR_KEYS; i ++) {
		switch (query_key(i))
		{
			case KEY_STATE_PRESS:
				Log("Call back press %s", key_name[i]);
				key_press_callback(get_keycode(i));
				wait_key(i);
				sti();
				ret = true;
			case KEY_STATE_RELEASE:
				Log("Call back release %s", key_name[i]);
				key_release_callback(get_keycode(i));
				clear_key(i);
				sti();
				ret = true;
		}
	}

	sti();
	return ret;
}

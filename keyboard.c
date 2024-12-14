/* File: keyboard.c
 * -----------------
 * This file includes all levels of functions implemented to 
 * enable the user to communicate with the pi/computer using a PS/2 device
 */
#include "keyboard.h"
#include "ps2.h"
#include "timer.h"
#include "ringbuffer.h"

static ps2_device_t *dev;
static keyboard_modifiers_t modifiers = 0;
bool keyboard_key_available(void);

void keyboard_init(gpio_id_t clock_gpio, gpio_id_t data_gpio) {
    dev = ps2_new(clock_gpio, data_gpio);
}

uint8_t keyboard_read_scancode(void) {
    return ps2_read(dev);
}

rb_t* keyboard_get_scancode_buffer(void) {
    return ps2_get_scancode_buffer(dev);
}


/*
 * Reads a sequence of scancode bytes corresponding to the press or
 * release of a single key. Returns a `key_action_t` struct that
 * represents the key action for the sequence read. Reads 1, 2, or 3 scancodes
*/
key_action_t keyboard_read_sequence(void) {
    key_action_t action = { 0 };  // should be this or pointer???
    action.what = KEY_PRESS; 
    uint8_t scancode;

    // can my infite loop be problematic??
    for (int i = 0; /* forever */; i++) {
        scancode = keyboard_read_scancode();
        if (scancode == 0xE0) { // extended key
            continue;
        } else if (scancode == 0xF0) { // break code for release
            action.what = KEY_RELEASE;
            continue;  
        } else {
            action.keycode = scancode;  // are we assumning no other codes possible??
            break;
        }
    }
    return action;
}

/*
 * The function reads (blocking) the next key event.
 * Returns a `key_event_t` struct that represents the key event.
 * A key event is a press or release of a single non-modifier key.
 * The returned struct includes the ps2 key that was pressed or
 * released and the keyboard modifiers in effect.
*/
key_event_t keyboard_read_event(void) {
    key_event_t event = { 0 };

    for (int i = 0; /* forever */; i++) { 
        key_action_t action = keyboard_read_sequence();
        event.key = ps2_keys[action.keycode];
    
        if (event.key.ch == PS2_KEY_SHIFT) {
            if (action.what == KEY_PRESS) {
                modifiers |= KEYBOARD_MOD_SHIFT;
            } else {
                modifiers &= ~KEYBOARD_MOD_SHIFT;
            }
            continue; 
        }
        else if (event.key.ch == PS2_KEY_ALT) {
            if (action.what == KEY_PRESS) {
                modifiers |= KEYBOARD_MOD_ALT;
            } else {
                modifiers &= ~KEYBOARD_MOD_ALT;
            }
            continue;
        }
        else if (event.key.ch == PS2_KEY_CTRL) {
            if (action.what == KEY_PRESS) {
                modifiers |= KEYBOARD_MOD_CTRL;
            } else {
                modifiers &= ~KEYBOARD_MOD_CTRL;
            }
            continue;
        }
        else if (event.key.ch == PS2_KEY_CAPS_LOCK) {
            if (action.what == KEY_PRESS) {
                modifiers ^= KEYBOARD_MOD_CAPS_LOCK;  // toggle
            }
            continue;  // ignore Caps Lock release
        }

        event.action = action;
        event.modifiers = modifiers;
        return event;  
    }
}

/*
 * This function reads (blocking) the next key typed on the keyboard.
 * The character returned reflects the current keyboard modifiers in effect
*/
char keyboard_read_next(void) {
    key_event_t event;
    while (event.action.what != KEY_PRESS) {
        event = keyboard_read_event();
    }

    ps2_key_t key = event.key;
    unsigned char ch = key.ch;
    unsigned char other_ch = key.other_ch;

    if (event.modifiers & KEYBOARD_MOD_SHIFT) {
        return other_ch;  
    }
    else if (event.modifiers & KEYBOARD_MOD_CAPS_LOCK) {
        if (ch >= 'a' && ch <= 'z') {
            return other_ch;
        }
    }
    return ch;  
}
bool keyboard_key_available(void) {
    return !ps2_rb_is_empty(dev);
}

key_event_t keyboard_read_event_timeout(unsigned long timeout_us) {
    key_event_t event = { 0 };  // Initialize with default values
    unsigned long start_time = timer_get_ticks();

    // Attempt to read within the timeout window
    while (timer_get_ticks() - start_time < timeout_us) {
        if (!ps2_rb_is_empty(dev)) {  // Check if there’s data in the buffer
            event = keyboard_read_event();  // Read the event
            if (event.action.what == KEY_PRESS) {
                return event;  // Return the key press event
            }
        }
    }

    // If timeout, return a "no event" struct
    return (key_event_t){ .action.what = NO_EVENT };
}

char keyboard_read_next_timeout(unsigned long timeout_us) {
    key_event_t event = keyboard_read_event_timeout(timeout_us);

    if (event.action.what == NO_EVENT) {
        return '\0';  // No key press detected
    }

    ps2_key_t key = event.key;
    unsigned char ch = key.ch;
    unsigned char other_ch = key.other_ch;

    if (event.modifiers & KEYBOARD_MOD_SHIFT) {
        return other_ch;  
    } else if (event.modifiers & KEYBOARD_MOD_CAPS_LOCK) {
        if (ch >= 'a' && ch <= 'z') {
            return other_ch;
        }
    }
    return ch;
}




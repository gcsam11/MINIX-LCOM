#ifndef _LAB3_KEYBOARD_H_
#define _LAB3_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

int(kbc_read_data)(uint8_t *_scancode);

int (restore_keyboard_interrupts)();

void (kbc_ih)();

#endif

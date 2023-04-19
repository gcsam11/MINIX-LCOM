#ifndef _LAB3_KEYBOARD_H_
#define _LAB3_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

int(kbd_read_data)(uint8_t *data);

void (kbc_ih)();

int(kbc_issue_cmd)(int kbc_cmd_reg, uint8_t cmd);

int (kbd_restore_interrupts)();

#endif

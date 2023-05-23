#ifndef PROJ_KEYBOARD_H
#define PROJ_KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "kbc_macros.h"

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

int(kbd_read_data)(uint8_t *data);

void (kbd_ih)();

int(kbc_issue_cmd)(int kbc_cmd_reg, uint8_t cmd);

#endif

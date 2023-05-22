#ifndef PROJ_KEYBOARD_H
#define PROJ_KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "keyboard_macros.h"
#include "kbc.h"
#include "kbc_macros.h"

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

int(kbd_read_data)(uint8_t *data);

void (kbd_ih)();

#endif

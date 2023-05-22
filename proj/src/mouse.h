#ifndef PROJ_MOUSE_H
#define PROJ_MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "mouse_macros.h"
#include "kbc.h"
#include "kbc_macros.h"

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int my_mouse_enable_data_reporting();

int my_mouse_disable_data_reporting();

void (mouse_ih)();

int(mouse_read_data)(uint8_t *data);

int(mouse_send_cmd)(uint8_t cmd);

void (mouse_parse_packet)(struct packet* pp);

#endif

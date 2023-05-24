#ifndef PROJ_TIMER_H
#define PROJ_TIMER_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "timer_macros.h"

int (timer_subscribe_int)(uint8_t *bit_no);

int (timer_unsubscribe_int)();

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

int (timer_get_conf)(uint8_t timer, uint8_t *st);

void (timer_int_handler)();

#endif

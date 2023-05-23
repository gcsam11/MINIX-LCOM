#ifndef PROJ_INTERRUPTS_H
#define PROJ_INTERRUPTS_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "devices/timer/timer.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"

int (subscribe_interrupts)();

int (unsubscribe_interrupts)();

#endif

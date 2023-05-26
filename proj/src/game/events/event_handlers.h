#ifndef PROJ_EVENT_HANDLERS_H
#define PROJ_EVENT_HANDLERS_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "../game_aux/game_aux.h"
#include "../../devices/keyboard/keyboard.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/timer/timer.h"
#include "../../sprites/sprite.h"
#include "../../sprites/pixelmaps/pixelmaps.h"

#define FREQUENCY 60.0
#define FRAME_RATE 60.0

void (kbd_event_handler)();

void (mouse_event_handler)();

void (timer_event_handler)();

#endif

#ifndef PROJ_EVENT_HANDLERS_H
#define PROJ_EVENT_HANDLERS_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "../game_aux/game_aux.h"
#include "../../devices/keyboard/keyboard.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/rtc/rtc.h"
#include "../../devices/timer/timer.h"
#include "../../sprites/sprite.h"
#include "../../sprites/pixelmaps/pixelmaps.h"

#define FREQUENCY 60.0
#define FRAME_RATE 60.0

/**
 * @brief Processes keyboard inputs for menu selections or character movement depending on game state
*/
void (kbd_event_handler)();

/**
 * @brief Processes mouse inputs for menu selections or mouse movement
*/
void (mouse_event_handler)();

/**
 * @brief Makes sprites move according to timer
*/
void (timer_event_handler)();

#endif

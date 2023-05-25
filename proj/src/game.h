#ifndef PROJ_GAME_H
#define PROJ_GAME_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "interrupts.h"
#include "devices/timer/timer.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/video_card/video_card.h"
#include "sprites/sprite.h"
#include "sprites/pixelmaps/pixelmaps.h"

#define FREQUENCY 60.0
#define FRAME_RATE 60.0

enum game_state_t {
    MENU,
    GAMEPLAY,
    DATE,
};

void (game_init)();

void (game_run)();

void (game_exit)();

void (render_frame)(Sprite** sprites);

void (set_game_state)(enum game_state_t state);

void (kbd_event_handler)();

void (mouse_event_handler)();

void (timer_event_handler)();

#endif

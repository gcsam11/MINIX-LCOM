#ifndef PROJ_GAME_AUX_H
#define PROJ_GAME_AUX_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/timer/timer.h"
#include "devices/video_card/video_card.h"
#include "sprites/sprite.h"
#include "sprites/pixelmaps/pixelmaps.h"

enum game_state_t {
    MENU,
    GAMEPLAY,
    DATE,
};

int (subscribe_interrupts)();

int (unsubscribe_interrupts)();

void (set_game_state)(enum game_state_t state);

void (clear_game_state)(enum game_state_t state);

void (render_frame)();

bool (check_hero_zombies_collisions)();

void (update_shots)();

void (manage_zombies_shot)();

void (manage_shots_at_edge)();

void (delete_zombie)(int pos);

void (delete_shot)(int pos);

#endif

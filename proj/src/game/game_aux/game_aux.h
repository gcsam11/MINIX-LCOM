#ifndef PROJ_GAME_AUX_H
#define PROJ_GAME_AUX_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "../../devices/keyboard/keyboard.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/timer/timer.h"
#include "../../devices/rtc/rtc.h"
#include "../../devices/video_card/video_card.h"
#include "../../sprites/sprite.h"
#include "../../sprites/pixelmaps/pixelmaps.h"
#include "../../devices/utils.h"

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

void (create_zombie_hord)();

bool (check_hero_zombies_collisions)();

bool (check_zombies_at_left_edge)();

bool (check_zombies_at_v_edge)();

void (update_shots)();

void (update_zombies)();

void (advance_zombie_hord)();

void (manage_zombies_shot)();

void (manage_shots_at_right_edge)();

void (delete_zombie)(int pos);

void (delete_shot)(int pos);

void get_date();

void draw_string(const char *string, uint16_t x, uint16_t y, uint8_t scale);

void draw_character(const char character, uint16_t x, uint16_t y, uint8_t scale);

void (update_score_pixmap)();

#endif

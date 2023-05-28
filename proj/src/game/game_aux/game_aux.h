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
};

/**
 * @brief Subscribes multiple interrupts
 * @return Returns 0 if case success, 1 if otherwise
*/
int (subscribe_interrupts)();

/**
 * @brief Unsubscribes multiple interrupts
 * @return Returns 0 if case success, 1 if otherwise
*/
int (unsubscribe_interrupts)();

/**
 * @brief Checks current game state and changes the screen accordingly
 * @param state Game state to be selected (Gameplay or Menu)
*/
void (set_game_state)(enum game_state_t state);

/**
 * @brief Checks current game state and clears the screen accordingly
 * @param state Game state to be selected (Gameplay or Menu)
*/
void (clear_game_state)(enum game_state_t state);

/**
 * @brief Clears current frame, draws loaded background and sprites present on sprite array
*/
void (render_frame)();

/**
 * @brief Creates zombie hords
*/
void (create_zombie_hord)();

/**
 * @brief Checks if hero sprite collides with any zombie sprites
 * @return Returns true if there is a collision, false otherwise
*/
bool (check_hero_zombies_collisions)();

/**
 * @brief Checks if zombies have reached the left part of the screen
 * @return Returns true if there is a collision, false otherwise
*/
bool (check_zombies_at_left_edge)();

/**
 * @brief Checks if zombies have reached the top or bottom part of the screen
 * @return Returns true if there is a collision, false otherwise
*/
bool (check_zombies_at_v_edge)();

/**
 * @brief Updates shot sprites positions
*/
void (update_shots)();

/**
 * @brief Updates zombies sprites positions
*/
void (update_zombies)();

/**
 * @brief Makes zombies go forward
*/
void (advance_zombie_hord)();

/**
 * @brief Checks for zombie collision with shots and deletes them if the collision exists
*/
void (manage_zombies_shot)();

/**
 * @brief Checks if the shots have reached the right part of the screen and deletes them if true
*/
void (manage_shots_at_right_edge)();

/**
 * @brief Deletes zombie in position pos
 * @param pos Zombie number
*/
void (delete_zombie)(int pos);

/**
 * @brief Deletes shot in position pos
 * @param pos Shot number
*/
void (delete_shot)(int pos);

/**
 * @brief Gets current date from RTC
*/
//void get_date();

/**
 * @brief Draws string
 * @param string String to be drawn
*/
void draw_string(const char *string, uint16_t x, uint16_t y, uint8_t scale);

/**
 * @brief Draws character
 * @param character Character to be drawn
*/
void draw_character(const char character, uint16_t x, uint16_t y, uint8_t scale);

/**
 * @brief Updates score on screen
*/
void (update_score_pixmap)();

void (update_date_pixmap)();

#endif

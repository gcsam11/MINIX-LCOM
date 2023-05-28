#ifndef PROJ_SPRITE_H
#define PROJ_SPRITE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "./devices/video_card/video_card.h"

typedef struct {
    uint16_t width, height;
    size_t size;
    int16_t x, y;
    int16_t vx, vy;
    uint16_t min_x, max_x, min_y, max_y;
    uint8_t *map;
    uint8_t *bytes;
} Sprite;

/**
 * @brief Creates sprite
 * @param xpm XPM to be loaded to the sprite
 * @return Returns the created sprite
*/
Sprite* (create_sprite)(xpm_map_t xpm, int16_t x, int16_t y, int16_t vx, int16_t vy);

/**
 * @brief Sets sprite pixelmap
 * @param new_xpm XPM to be loaded to sprite
*/
void (set_sprite_pixelmap)(Sprite* sp, xpm_map_t new_xpm);

/**
 * @brief Sets sprite x position
 * @param new_x New x position
*/
void (set_sprite_x)(Sprite* sp, int16_t new_x);

/**
 * @brief Sets sprite y position
 * @param new_y New y position
*/
void (set_sprite_y)(Sprite* sp, int16_t new_y);

/**
 * @brief Sets sprite x velocity
 * @param new_vx New x velocity
*/
void (set_sprite_vx)(Sprite* sp, int16_t new_vx);

/**
 * @brief Sets sprite y velocity
 * @param new_vy New y velocity
*/
void (set_sprite_vy)(Sprite* sp, int16_t new_vy);

/**
 * @brief Updates sprite position
*/
void (update_sprite_position)(Sprite* sp);

/**
 * @brief Moves sprite left
 * @param dis Distance to move
*/
void (move_sprite_left)(Sprite* sp, uint16_t dis);

/**
 * @brief Checks if sprite is at the right edge of the screen
*/
bool (sprite_at_right_edge)(Sprite* sp);

/**
 * @brief Checks if sprite is at the left edge of the screen
*/
bool (sprite_at_left_edge)(Sprite* sp);

/**
 * @brief Checks if sprite is at the top edge of the screen
*/
bool (sprite_at_top_edge)(Sprite* sp);

/**
 * @brief Checks if sprite is at the bottom edge of the screen
*/
bool (sprite_at_bottom_edge)(Sprite* sp);

/**
 * @brief Checks if two sprites are colliding
*/
bool (check_sprite_collision)(Sprite* sp1, Sprite* sp2);

/**
 * @brief Draws sprite
*/
void (draw_sprite)(Sprite* sp);

/**
 * @brief Clears sprite's pixelmap
*/
void (clear_sprite)(Sprite* sp);

/**
 * @brief Destroys sprite pointer
*/
void (destroy_sprite)(Sprite** sp);

#endif

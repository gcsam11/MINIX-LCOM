#ifndef PROJ_SPRITE_H
#define PROJ_SPRITE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "./devices/video_card/video_card.h"

typedef struct {
    uint16_t width, height;
    int16_t x, y;
    int16_t vx, vy;
    uint16_t min_x, max_x, min_y, max_y;
    uint8_t *map;
} Sprite;

Sprite* (create_sprite)(xpm_map_t xpm, int16_t x, int16_t y, int16_t vx, int16_t vy);

void (set_sprite_pixelmap)(Sprite* sp, xpm_map_t new_xpm);

void (set_sprite_x)(Sprite* sp, int16_t new_x);

void (set_sprite_y)(Sprite* sp, int16_t new_y);

void (set_sprite_vx)(Sprite* sp, int16_t new_vx);

void (set_sprite_vy)(Sprite* sp, int16_t new_vy);

void (update_sprite_position)(Sprite* sp);

bool (sprite_at_left_edge)(Sprite* sp);

bool (check_sprite_collision)(Sprite* sp1, Sprite* sp2);

void (draw_sprite)(Sprite* sp);

void (clear_sprite)(Sprite* sp);

void (destroy_sprite)(Sprite** sp);

#endif

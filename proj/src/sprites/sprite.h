#ifndef PROJ_SPRITE_H
#define PROJ_SPRITE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "./devices/video_card/video_card.h"

typedef struct {
    int16_t x, y;
    int16_t vx, vy;
    uint16_t width, height;
    uint16_t min_x, max_x, min_y, max_y;
    uint8_t *map;
} Sprite;

Sprite* (create_sprite)(xpm_map_t xpm, int16_t x, int16_t y, int16_t vx, int16_t vy);

void (set_sprite_vx)(Sprite* sp, int16_t new_vx);

void (set_sprite_vy)(Sprite* sp, int16_t new_vy);

void (update_sprite_position)(Sprite* sp);

void (draw_sprite)(Sprite* sp);

void (clear_sprite)(Sprite* sp);

void (destroy_sprite)(Sprite** sp);

#endif

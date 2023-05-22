#ifndef PROJ_SPRITE_H
#define PROJ_SPRITE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "video_card.h"

typedef struct {
    double x, y;
    double prev_x, prev_y;
    double vx, vy;
    uint16_t width, height;
    double min_x, max_x, min_y, max_y;
    uint8_t *map;
} Sprite;

Sprite *create_sprite(xpm_map_t xpm, double x, double y, double vx, double vy);

void set_sprite_vx(Sprite *sp, double new_vx);

void set_sprite_vy(Sprite *sp, double new_vy);

void update_sprite_position(Sprite *sp, double delta_t);

void draw_sprite(Sprite *sp);

void clear_sprite(Sprite *sp);

void destroy_sprite(Sprite **sp);

#endif

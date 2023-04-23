#ifndef PROJ_SPRITE_H
#define PROJ_SPRITE_H

#include <lcom/lcf.h>

#include "video_card.h"

typedef struct {
    int x, y;
    uint16_t width, height;
    int xspeed, yspeed;
    uint8_t *map;
} Sprite;

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed);

int sprite_upper_bound(Sprite *sp);

int sprite_lower_bound(Sprite *sp);

int sprite_left_bound(Sprite *sp);

int sprite_right_bound(Sprite *sp);

bool sprite_touches_top(Sprite* sp);

bool sprite_touches_bottom(Sprite* sp);

bool sprite_touches_left(Sprite* sp);

bool sprite_touches_right(Sprite* sp);

void draw_sprite(Sprite *sp);

void clear_sprite(Sprite *sp);

void update_sprite_position(Sprite *sp);

void set_sprite_xspeed(Sprite *sp, int new_xspeed);

void set_sprite_yspeed(Sprite *sp, int new_yspeed);

void destroy_sprite(Sprite **sp);

#endif

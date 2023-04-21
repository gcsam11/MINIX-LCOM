#ifndef _PROJ_SPRITE_H_
#define _PROJ_SPRITE_H_

#include <lcom/lcf.h>

#include "video_card.h"

typedef struct {
    int x, y;
    uint16_t width, height;
    int xspeed, yspeed;
    uint8_t *map;
} Sprite;

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed);

void draw_sprite(Sprite *sp);

void clear_sprite(Sprite *sp);

void update_sprite_position(Sprite *sp);

void destroy_sprite(Sprite **sp);

#endif

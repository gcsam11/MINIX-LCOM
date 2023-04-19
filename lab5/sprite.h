#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include <lcom/lcf.h>

typedef struct {
    int x, y; // current position
    int width, height; // dimensions
    int xspeed, yspeed; // current speed
    uint8_t *map; // the pixmap
} Sprite;

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed);

void update_sprite(Sprite *sp);

void destroy_sprite(Sprite *sp);

#endif

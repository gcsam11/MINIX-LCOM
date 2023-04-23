#include "sprite.h"

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed) {
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;

    if( sp == NULL ) return NULL;
    
    sp->map = xpm_load(xpm, XPM_8_8_8, &img);

    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }

    sp->width = img.width;
    sp->height = img.height;
    sp->x = x;
    sp->y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;

    return sp;
}

void draw_sprite(Sprite *sp) {
    vg_draw_pixel_map(sp->x, sp->y, sp->width, sp->height, sp->map);
}

void clear_sprite(Sprite *sp) {
    vg_clear_pixel_map(sp->x, sp->y, sp->width, sp->height);
}

void update_sprite_position(Sprite *sp) {
    sp->x += sp->xspeed;
    sp->y += sp->yspeed;
}

void set_sprite_xspeed(Sprite *sp, int new_xspeed) {
    sp->xspeed = new_xspeed;
}

void set_sprite_yspeed(Sprite *sp, int new_yspeed) {
    sp->yspeed = new_yspeed;
}

void destroy_sprite(Sprite **sp) {
    if(*sp == NULL) return;
    if((*sp)->map) free((*sp)->map);
    free(*sp);
    *sp = NULL;
}

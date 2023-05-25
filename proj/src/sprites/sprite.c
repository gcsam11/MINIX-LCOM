#include "sprite.h"

Sprite* (create_sprite)(xpm_map_t xpm, int16_t x, int16_t y, int16_t vx, int16_t vy) {
    Sprite* sp = (Sprite*) malloc (sizeof(Sprite));
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
    sp->vx = vx;
    sp->vy = vy;
    sp->min_x = 0;
    sp->max_x = get_h_res() - sp->width;
    sp->min_y = 0;
    sp->max_y = get_v_res() - sp->height;

    return sp;
}

void (set_sprite_vx)(Sprite* sp, int16_t new_vx) {
    sp->vx = new_vx;
}

void (set_sprite_vy)(Sprite* sp, int16_t new_vy) {
    sp->vy = new_vy;
}

void (update_sprite_position)(Sprite* sp) {
    sp->x += sp->vx;
    sp->y += sp->vy;

    if (sp->x > sp->max_x) sp->x = sp->max_x;
    if (sp->x < sp->min_x) sp->x = sp->min_x;
    if (sp->y > sp->max_y) sp->y = sp->max_y;
    if (sp->y < sp->min_y) sp->y = sp->min_y;
}

void (draw_sprite)(Sprite* sp) {
    vg_draw_pixel_map(sp->x, sp->y, sp->width, sp->height, sp->map);
}

void (clear_sprite)(Sprite* sp) {
    vg_clear_pixel_map(sp->y, sp->y, sp->width, sp->height);
}

void (destroy_sprite)(Sprite** sp) {
    if(*sp == NULL) return;
    if((*sp)->map) free((*sp)->map);
    free(*sp);
    *sp = NULL;
}

#include "sprite.h"

Sprite* (create_sprite)(xpm_map_t xpm, int16_t x, int16_t y, int16_t vx, int16_t vy) {
    Sprite* sp = (Sprite*) malloc (sizeof(Sprite));

    if( sp == NULL ) return NULL;
    
    xpm_image_t img;
    sp->map = xpm_load(xpm, XPM_8_8_8, &img);

    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }

    sp->width = img.width;
    sp->height = img.height;
    sp->size = img.size;

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

void (set_sprite_x)(Sprite* sp, int16_t new_x) {
    sp->x = new_x;
}

void (set_sprite_y)(Sprite* sp, int16_t new_y) {
    sp->y = new_y;
}

void (set_sprite_vx)(Sprite* sp, int16_t new_vx) {
    sp->vx = new_vx;
}

void (set_sprite_vy)(Sprite* sp, int16_t new_vy) {
    sp->vy = new_vy;
}

void (set_sprite_pixelmap)(Sprite* sp, xpm_map_t new_xpm) {
    if(sp->map) free(sp->map);
    
    xpm_image_t img;
    sp->map = xpm_load(new_xpm, XPM_8_8_8, &img);
    sp->width = img.width;
    sp->height = img.height;
}

bool (sprite_at_right_edge)(Sprite* sp) {
    return sp->x == sp->max_x;
}

bool (sprite_at_left_edge)(Sprite* sp) {
    return sp->x == 0;
}

bool (sprite_at_top_edge)(Sprite* sp) {
    return sp->y == 0;
}

bool (sprite_at_bottom_edge)(Sprite* sp) {
    return sp->y == sp->max_y;
}

void (update_sprite_position)(Sprite* sp) {
    sp->x += sp->vx;
    sp->y += sp->vy;

    if (sp->x > sp->max_x) sp->x = sp->max_x;
    if (sp->x < sp->min_x) sp->x = sp->min_x;
    if (sp->y > sp->max_y) sp->y = sp->max_y;
    if (sp->y < sp->min_y) sp->y = sp->min_y;
}

void (move_sprite_left)(Sprite* sp, uint16_t dis) {
    sp->x -= dis;
    
    if (sp->x < sp->min_x) sp->x = sp->min_x;
}

bool (check_sprite_collision)(Sprite* sp1, Sprite* sp2) {
    int16_t top1 = sp1->y;
    int16_t bottom1 = (sp1->y + sp1->height);
    int16_t right1 = (sp1->x + sp1->width);
    int16_t left1 = sp1->x;

    int16_t top2 = sp2->y;
    int16_t bottom2 = (sp2->y + sp2->height);
    int16_t right2 = (sp2->x + sp2->width);
    int16_t left2 = sp2->x;

    bool overlap_top_bottom = top1 < bottom2;
    bool overlap_bottom_top = bottom1 > top2;
    bool overlap_right_left = right1 > left2;
    bool overlap_left_right = left1 < right2;

    return (overlap_top_bottom && overlap_bottom_top && overlap_right_left && overlap_left_right);
}

void (draw_sprite)(Sprite* sp) {
    vg_draw_pixel_map(sp->x, sp->y, sp->width, sp->height, sp->map);
}

void (clear_sprite)(Sprite* sp) {
    vg_clear_pixel_map(sp->y, sp->y, sp->width, sp->height);
}

void destroy_sprite(Sprite** sp) {
    if((*sp) == NULL)
        return;

    if((*sp)->map)
        free((*sp)->map);
        
    free(*sp);
    *sp = NULL;
}

#include "sprite.h"
#include "video_gr.h"

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;

    if( sp == NULL )
        return NULL;
    
    // read the sprite pixmap
    sp->map = xpm_load(xpm, XPM_INDEXED, &img);

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

void update_sprite(Sprite *sp) {
    vg_clear_xpm(sp->x, sp->y, sp->width, sp->height);

    sp->x += sp->xspeed;
    sp->y += sp->yspeed;

    vg_draw_xpm(sp->x, sp->y, sp->width, sp->height, sp->map);
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL )
        return;
    if( sp ->map )
        free(sp->map);
    free(sp);
    sp = NULL; // XXX: pointer is passed by value
    // should do this @ the caller
}

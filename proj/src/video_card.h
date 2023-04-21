#ifndef _PROJ_VIDEO_CARD_H_
#define _PROJ_VIDEO_CARD_H_

#include <lcom/lcf.h>

#include "video_card_macros.h"

void* (vg_init)(uint16_t mode);

int (vg_draw_pixel_map)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h, uint8_t* map);

int (vg_clear_pixel_map)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h);

int(vg_draw_pixel)(uint8_t** ptr, uint32_t color);

#endif


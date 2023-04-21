#ifndef _LCOM_VIDEO_CARD_H_
#define _LCOM_VIDEO_CARD_H_

#include <lcom/lcf.h>

#include "macros.h"

void* (vg_init)(uint16_t mode);

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rect_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

int (vg_draw_xpm)(uint16_t x, uint16_t y, int width, int height, uint8_t* map);

int (vg_clear_xpm)(uint16_t x, uint16_t y, int width, int height);

int(vg_draw_pixel)(uint8_t** ptr, uint32_t color);

#endif

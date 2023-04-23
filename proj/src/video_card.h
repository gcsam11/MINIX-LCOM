#ifndef PROJ_VIDEO_CARD_H
#define PROJ_VIDEO_CARD_H

#include <lcom/lcf.h>

uint16_t get_h_res();

uint16_t get_v_res();

int my_vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p);

void* (vg_init)(uint16_t mode);

void swap_buffers();

int (vg_draw_pixel_map)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h, uint8_t* map);

int (vg_clear_pixel_map)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h);

int(vg_draw_pixel)(uint8_t** ptr, uint32_t color);

#endif

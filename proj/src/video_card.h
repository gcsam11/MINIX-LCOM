#ifndef PROJ_VIDEO_CARD_H
#define PROJ_VIDEO_CARD_H

#include <lcom/lcf.h>
#include <stdint.h>

uint16_t get_h_res();

uint16_t get_v_res();

int my_vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p);

void* (vg_init)(uint16_t mode);

int page_flip();

void (vg_set_background)(xpm_map_t xpm);

void (vg_draw_background)(); 

void vg_clear_frame();

int (vg_draw_pixel_map)(uint16_t x, uint16_t y, uint16_t xpm_image_w, uint16_t xpm_image_h, uint8_t* map);

int (vg_clear_pixel_map)(uint16_t x, uint16_t y, uint16_t xpm_image_w, uint16_t xpm_image_h);

void (vg_draw_pixel)(uint8_t** ptr, uint32_t* color_ptr);

#endif

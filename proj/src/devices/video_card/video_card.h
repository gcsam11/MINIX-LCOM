#ifndef PROJ_VIDEO_CARD_H
#define PROJ_VIDEO_CARD_H

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief This function returns the horizontal resolution
 * @return Returns horizontal resolution
*/
uint16_t (get_h_res)();

/**
 * @brief This function returns the vertical resolution
 * @return Returns vertical resolution
*/
uint16_t (get_v_res)();

/**
 * @brief This function allocates memory necessary for the graphics
 * @param mode The VBE mode in which the graphics will be configured
 * @return Return 0 in case of success, 1 if otherwise
 */
int (my_vbe_get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p);

/**
 * @brief This funtion initiates graphics mode (GUI instead of CLI)
 * @param mode The VBE mode in which the graphics will be configured
 * @return Return 0 in case of success, 1 if otherwise
 */
void* (vg_init)(uint16_t mode);

/**
 * @brief This function is used to set memory for page flipping
 * @return Return 0 in case of success, 1 if otherwise
*/
int (page_flip)();

/**
 * @brief This function is used to load the xpm for the background
 * @param xpm The XPM that contains the background that will be drawn
*/
void (vg_set_background)(xpm_map_t xpm);

/**
 * @brief This function draws the background that should already be loaded
*/
void (vg_draw_background)(); 

/**
 * @brief This function clears the background frame
*/
void (vg_clear_frame)();

/**
 * @brief draws pixel map
*/
int (vg_draw_pixel_map)(uint16_t x, uint16_t y, uint16_t xpm_image_w, uint16_t xpm_image_h, uint8_t* map);

/**
 * @brief clears pixel map
*/
int (vg_clear_pixel_map)(uint16_t x, uint16_t y, uint16_t xpm_image_w, uint16_t xpm_image_h);

/**
 * @brief draws a pixel 
*/
void (vg_draw_pixel)(uint8_t** ptr, uint32_t* color_ptr);

#endif

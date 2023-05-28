#ifndef PROJ_UTILS_H
#define PROJ_UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>

int (util_sys_inb)(int port, uint8_t *value);

int(util_get_LSB)(uint16_t val, uint8_t *lsb);

int(util_get_MSB)(uint16_t val, uint8_t *msb);

bool is_alpha(char character);

#endif

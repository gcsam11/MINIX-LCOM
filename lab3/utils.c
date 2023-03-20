#include <stdint.h>

#include "utils.h"

#ifdef _LAB3_UTILS_H_
uint32_t sys_inb_cnt = 0;
#endif

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val >> 8);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  #ifdef _LAB3_UTILS_H_
  sys_inb_cnt++;
  #endif
  
  uint32_t val = 0;

  if (sys_inb(port,  &val) != F_OK) {
    printf("AN ERROR OCURRED WHEN READING FROM THE PORT USING sys_inb\n");
    return 1;
  }
  
  *value = (uint8_t)val;

  return 0;
}

#include "utils.h"

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val = 0;

  if (sys_inb(port,  &val) != OK) {
    printf("UNABLE TO READ FROM THE PORT USING 'sys_inb'\n");
    return 1;
  }
  
  *value = (uint8_t)val;

  return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val >> 8);
  return 0;
}

bool is_alpha(char character) {
    if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z'))
        return true;

    return false;
}

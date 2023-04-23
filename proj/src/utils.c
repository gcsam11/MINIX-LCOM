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

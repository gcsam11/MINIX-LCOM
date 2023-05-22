#include "kbc.h"

int(kbc_issue_cmd)(int kbc_cmd_reg, uint8_t cmd) {
  uint8_t st = 0;
  uint8_t tries = 0;

  while (tries < 4) {

    if (util_sys_inb(KBC_ST_REG, &st) != OK) {
      printf("UNABLE TO READ THE KBC STATUS\n");
      return 1;
    }

    if ((st & KBC_STREG_IBF) == 0) {
      sys_outb(kbc_cmd_reg, cmd);
      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    tries++;
  };

  return 1;
}

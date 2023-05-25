#include "keyboard.h"

int kbd_hook_id = 1;

uint8_t scancode = 0;
int byte_cnt = 0;
uint8_t scan_code_bytes[2];

int (kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(kbd_hook_id);

  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != F_OK) {
    printf("FAILED TO SET THE KEYBOARD IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

int (kbd_unsubscribe_int)() {
  
  if (sys_irqrmpolicy(&kbd_hook_id) != OK) {
    printf("FAILED TO REMOVE KEYBOARD IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

void (kbd_int_handler)() {
    if (kbd_read_data(&scancode) != OK) {
      printf("UNABLE TO READ THE KEYBOARD'S SCANCODE\n");
    }

    if (scancode == FIRSTBYTE_2BYTECODE) {
      scan_code_bytes[byte_cnt++] = scancode;
      return;
    }

    scan_code_bytes[byte_cnt] = scancode;
    byte_cnt = 0;
}

int(kbd_read_data)(uint8_t *data) {
  uint8_t st = 0;

  if (util_sys_inb(KBC_ST_REG, &st) != OK) {
    printf("UNABLE TO READ THE KBC STATUS\n");
    return 1;
  }

  if ((st & KBC_STREG_OBF) == KBC_STREG_OBF) {
    if (util_sys_inb(KBC_OUT_BUF, data) != OK) {
      printf("ERROR, CANNOT READ FROM THE KBC OUTPUT BUFFER WHILE IT'S NOT YET FULL\n");
      return 1;
    }

    if (((st & (KBC_STREG_PARITY | KBC_STREG_TIMEOUT)) == 0) && ((st & KBC_STREG_AUX) == 0)) {
      return 0;
    } else {
      printf("ERROR WHEN READING KEYBOARD DATA. EITHER TIMEOUT, PARITY OR RECEIVED MOUSE DATA\n");
      return 1;
    }
  }
  return 1;
}

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

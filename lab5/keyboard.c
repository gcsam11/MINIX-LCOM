#include "keyboard.h"

uint8_t scancode = 0;
int kbd_hook_id = KBC_IRQ;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(kbd_hook_id);

  if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != F_OK) {
    printf("FAILED TO SET KEYBOARD IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

int (kbd_unsubscribe_int)() {
  
  if (sys_irqrmpolicy(&kbd_hook_id) != F_OK) {
    printf("FAILED TO REMOVE KEYBOARD IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

int(kbd_read_data)(uint8_t *data) {
  uint8_t st = 0;

  if (util_sys_inb(KBC_ST_REG, &st) != F_OK) {
    printf("ERROR WHILE READING THE KBC STATUS\n");
    return 1;
  }

  if ((st & KBC_STREG_OBF) == KBC_STREG_OBF) {
    if (util_sys_inb(KBC_OUT_BUF, data) != F_OK) {
      printf("ERROR WHILE READING KBC DATA WITH FULL BUFFER\n");
      return 1;
    }

    if (((st & (KBC_STREG_PARITY | KBC_STREG_TIMEOUT)) == 0) && ((st & KBC_STREG_AUX) == 0)) {
      return 0;
    } else {
      printf("ERROR. EITHER TIMEOUT, PARITY OR RECEIVED MOUSE DATA\n");
      return 1;
    }
  }
  return 1;
}

void (kbc_ih)() {
    if (kbd_read_data(&scancode) != F_OK) {
        printf("ERROR WHILE READING THE SCANCODE\n");
    }
}

int(kbc_issue_cmd)(int kbc_cmd_reg, uint8_t cmd) {
  uint8_t st = 0;
  uint8_t tries = 0;

  while (tries < 4) {

    if (util_sys_inb(KBC_ST_REG, &st) != F_OK) {
      printf("ERROR WHILE READING THE KBC STATUS\n");
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

int (kbd_restore_interrupts)() {
  uint8_t cmd;

  kbc_issue_cmd(KBC_IN_BUF_CMDS, KBC_READ_CMD_BT);
  util_sys_inb(KBC_OUT_BUF, &cmd);

  cmd = cmd | KBC_INT;

  kbc_issue_cmd(KBC_IN_BUF_CMDS, KBC_WRITE_CMD_BT);
  kbc_issue_cmd(KBC_IN_BUF_ARGS, cmd);
  return 0;
}

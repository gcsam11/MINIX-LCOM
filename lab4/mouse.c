#include "mouse.h"
#include "i8042.h"

int mouse_hook_id = 2;
uint8_t pckt_bt = 0;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(mouse_hook_id);

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != F_OK) {
    printf("FAILED TO SET MOUSE IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

int (mouse_unsubscribe_int)() {
  
  if (sys_irqrmpolicy(&mouse_hook_id) != F_OK) {
    printf("FAILED TO REMOVE MOUSE IRQ POLICY\n");
    return 1;
  }
  
  return 0;
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

int(mouse_send_cmd)(uint8_t cmd) {
  uint8_t verify_ack = 0;

  do {
    
    if (kbc_issue_cmd(KBC_IN_BUF_CMDS, KBC_WRITE_BT_MOUSE) != F_OK) {
      printf("ERROR, ISSUING COMMAND TO KBC FAILED\n");
      return 1;
    }
    if (kbc_issue_cmd(KBC_IN_BUF_ARGS, cmd) != F_OK) {
      printf("ERROR, ISSUING COMMAND TO KBC FAILED\n");
      return 1;
    }
    if (util_sys_inb(KBC_IN_BUF_ARGS, &verify_ack) != F_OK) {
      printf("ERROR WHILE READING MOUSE ACKNOWLEDGEMENT BYTE\n");
      return 1;
    }

  } while (verify_ack != MOUSE_ACK);

  return 0;
}

int my_mouse_enable_data_reporting() {
    if (mouse_send_cmd(MOUSE_ENBL_DT_RPORT) != F_OK) {
      printf("ERROR WHILE SENDING MOUSE COMMAND\n");
      return 1;
    }
    
    return 0;
}

int my_mouse_disable_data_reporting() {
    if (mouse_send_cmd(MOUSE_DISBL_DT_RPORT) != F_OK) {
      printf("ERROR WHILE SENDING MOUSE COMMAND\n");
      return 1;
    }

    return 0;
}

int(mouse_read_data)(uint8_t *data) {
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

    if (((st & KBC_STREG_AUX) == KBC_STREG_AUX) && ((st & (KBC_STREG_PARITY | KBC_STREG_TIMEOUT)) == 0)) {
      return 0;
    } else {
      printf("ERROR. EITHER DIDN'T RECEIVE MOUSE DATA, TIMEOUT, OR PARITY\n");
      return 1;
    }
  }
  printf("ERROR, OUTPUT BUFFER IS NOT YET FULL\n");
  return 1;
}

void (mouse_ih)() {
    if (mouse_read_data(&pckt_bt) != F_OK) {
        printf("ERROR WHILE READING THE PACKET BYTE\n");
    }
}

void (mouse_parse_packet)(struct packet* pp) {
  uint8_t first_pckt_bt = pp->bytes[0];

  pp->lb = first_pckt_bt & MOUSE_PCKT_LB;
  pp->rb = first_pckt_bt & MOUSE_PCKT_RB;
  pp->mb = first_pckt_bt & MOUSE_PCKT_MB;
  pp->delta_x = (first_pckt_bt & MOUSE_PCKT_MSBX_DEL) ? (0xFF00 | pp->bytes[1]) : (pp->bytes[1]);
  pp->delta_y = (first_pckt_bt & MOUSE_PCKT_MSBY_DEL) ? (0xFF00 | pp->bytes[2]) : (pp->bytes[2]);
  pp->x_ov = first_pckt_bt & MOUSE_PCKT_XOVFL;
  pp->y_ov = first_pckt_bt & MOUSE_PCKT_YOVFL;
}

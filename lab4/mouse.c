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

int (*eht[])(struct Gesture*, int16_t, int16_t, uint8_t, uint8_t) = {handle_LB_PRESSED, handle_LB_RELEASED, handle_RB_PRESSED, handle_RB_RELEASED, handle_BUTTON_EV, handle_MOUSE_MOV};

int (mouse_gesture_matching)(struct Gesture *gesture, struct mouse_ev *event, uint8_t x_len, uint8_t tolerance) {
  return (eht[event->type])(gesture, event->delta_x, event->delta_y, x_len, tolerance);
}

int (handle_LB_PRESSED)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance) {
  switch (gesture->state) {
    case INITIAL:
      printf("\nEVENT: LB PRESSED, STATE: INITIAL\n");
      gesture->delta_x = 0;
      gesture->delta_y = 0;
      gesture->state = LEFT_SLOPE;
      break;
    case VERTEX:
      printf("\nEVENT: LB PRESSED, STATE: VERTEX\n");
      gesture->delta_x = 0;
      gesture->delta_y = 0;
      gesture->state = LEFT_SLOPE;
      break;
    case FINISHED:
      printf("\nEVENT: LB PRESSED, STATE: FINISHED\n");
      return 0;
    default:
      gesture->state = INITIAL;
      break;
  }
  return 1;
}

int (handle_LB_RELEASED)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance) {
  switch (gesture->state) {
    case LEFT_SLOPE:
      printf("\nEVENT: LB RELEASED, STATE: LEFT_SLOPE\n");
      if((gesture->delta_x >= x_len) && (gesture->delta_y/gesture->delta_x) > 1) {
          gesture->delta_x = 0;
          gesture->delta_y = 0;
          gesture->state = VERTEX;
        } else gesture->state = INITIAL;
      break;
    case FINISHED:
      printf("\nEVENT: LB RELEASED, STATE: FINISHED\n");
      return 0;
    default:
      gesture->state = INITIAL;
      break;
  }
  return 1;
}

int (handle_RB_PRESSED)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance) {
  switch (gesture->state) {
    case VERTEX:
      printf("\nEVENT: RB PRESSED, STATE: VERTEX\n");
      if(abs(gesture->delta_x) > tolerance || abs(gesture->delta_y) > tolerance) {
          gesture->state = INITIAL;
        } else {
            gesture->delta_x = 0;
            gesture->delta_y = 0;
            gesture->state = RIGHT_SLOPE;
          }
      break;
    case FINISHED:
      printf("\nEVENT: RB PRESSED, STATE: FINISHED\n");
      return 0;
    default:
      gesture->state = INITIAL;
      break;
  }
  return 1;
}

int (handle_RB_RELEASED)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance) {
  switch (gesture->state) {
    case RIGHT_SLOPE:
      printf("\nEVENT: RB RELEASED, STATE: RIGHT_SLOPE\n");
      if((gesture->delta_x >= x_len) && (gesture->delta_y/gesture->delta_x) < -1) {
          gesture->state = FINISHED;
          return 0;
        } else gesture->state = INITIAL;
      break;
    case FINISHED:
      printf("\nEVENT: RB RELEASED, STATE: FINISHED\n");
      return 0;
    default:
      gesture->state = INITIAL;
      break;
  }
  return 1;
}

int (handle_BUTTON_EV)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance) {
  switch (gesture->state) {
    case FINISHED:
      printf("\nEVENT: BUTTON EV, STATE: FINISHED\n");
      return 0;
    default:
      gesture->state = INITIAL;
      break;
  }

  return 1;
}

int (handle_MOUSE_MOV)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance) {
  switch (gesture->state) {
    case LEFT_SLOPE:
      printf("\nEVENT: MOUSE MOV, STATE: LEFT_SLOPE\n");
      if(ev_delta_x <= 0 || ev_delta_y <= 0) {
          if(abs(ev_delta_x) > tolerance || abs(ev_delta_y) > tolerance){
            gesture->state = INITIAL;
            break;
          }
        }
      if(ev_delta_x != 0 && (ev_delta_y)/(ev_delta_x) <= 1) {
        gesture->state = INITIAL;
        } else {
            gesture->delta_x += ev_delta_x;
            gesture->delta_y += ev_delta_y;
          }
      break;
    case VERTEX:
      printf("\nEVENT: MOUSE MOV, STATE: VERTEX\n");
      gesture->delta_x += ev_delta_x;
      gesture->delta_y += ev_delta_y;
      break;
    case RIGHT_SLOPE:
      printf("\nEVENT: MOUSE MOV, STATE: RIGHT_SLOPE\n");
      if(ev_delta_x <= 0 || ev_delta_y >= 0) {
          if(abs(ev_delta_x) > tolerance || abs(ev_delta_y) > tolerance){
            gesture->state = INITIAL;
            break;
          }
        }
      if(ev_delta_x != 0 && (ev_delta_y)/(ev_delta_x) >= -1) {
        gesture->state = INITIAL;
        } else {
            gesture->delta_x += ev_delta_x;
            gesture->delta_y += ev_delta_y;
        }
      break;
    case FINISHED:
      printf("\nEVENT: MOUSE MOV, STATE: FINISHED\n");
      return 0;
    default:
      gesture->state = INITIAL;
      break;
  }
  return 1;
}

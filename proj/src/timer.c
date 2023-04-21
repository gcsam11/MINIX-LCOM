#include "timer.h"

int hook_id = 0;
uint32_t interrupt_cnt = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hook_id);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK) {
    printf("FAILED TO SET THE TIMER IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != F_OK) {
    printf("FAILED TO REMOVE THE TIMER IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

void (timer_int_handler)() {
  interrupt_cnt++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, rb_cmd) != F_OK) {
    printf("AN ERROR OCURRED WHEN WRITTING THE READ_BACK COMMAND TO THE TIMER CONTROL PORT\n");
    return 1;
  }
  
  if (util_sys_inb(TIMER_N(timer), st) != F_OK) {
    printf("AN ERROR OCURRED WHEN READING THE STATUS BYTE FROM THE DESIRED TIMER\n");
    return 1;
  }
  
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val config;

  switch (field) {
    case tsf_all:
      config.byte = st;
      break;

    case tsf_initial:
      config.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;

    case tsf_mode:
      config.count_mode = (st & 0xE) >> 1;

      if (config.count_mode > 5) {
        config.count_mode ^= BIT(2);
      }
      break;

    case tsf_base:
      config.bcd = st & TIMER_BCD;
      break;  
    
    default:
      printf("THE STATUS FIELD RECEIVED IS INCORRECT\n");
      return 1;
  }

  if (timer_print_config(timer, field, config) != OK) {
    printf ("AN ERROR OCURRED WHEN TRYING TO PRINT THE CONFIGURATION\n");
    return 1;
  }

  return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st = 0;

  if(freq < 19 || freq > TIMER_MAX_FREQ) {
    printf("ERROR, DESIRED FREQUENCY OUT OF THE VALID RANGE\n");
    return 1;
  }

  if (timer_get_conf(timer, &st) != OK) {
    printf("AN ERROR OCURRED WHEN GETTING THE CONFIGURATION OF THE TIMER");
    return 1;
  }
  
  uint8_t ctrl_wrd = TIMER_SEL_N(timer) | TIMER_LSB_MSB | (st & 0x0F);

  if(sys_outb(TIMER_CTRL, (uint32_t)ctrl_wrd) != OK) {
      printf("AN ERROR OCURRED WHEN WRITTING THE CONTROL WORD TO THE TIMER CONTROL PORT\n");
      return 1;
  }

  uint16_t div = TIMER_MAX_FREQ / freq;

  uint8_t lsb_div = 0;
  uint8_t msb_div = 0;

  util_get_LSB(div, &lsb_div);
  util_get_MSB(div, &msb_div);

  if(sys_outb(TIMER_N(timer), lsb_div) != OK) {
      printf("AN ERROR OCURRED WHEN WRITTING LSB OF DIV TO THE TIMER\n");
      return 1;
  }

  if(sys_outb(TIMER_N(timer), msb_div) != OK) {
      printf("AN ERROR OCURRED WHEN WRITTING MSB OF DIV TO THE TIMER\n");
      return 1;
  }

  return 0;
}

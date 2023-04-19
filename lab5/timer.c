#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = TIMER0_IRQ;
uint32_t timer0_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t timer_selection = 0;
  uint8_t timer_port = 0;
  uint8_t st = 0;

  if(freq < 19 || freq > TIMER_FREQ) {
    printf("ERROR, FRQUENCY OUT OF VALID RANGE\n");
    return 1;
  }

  if (timer == 0) {

    timer_selection = TIMER_SEL0;
    timer_port = TIMER_0;

  } else if (timer == 1) {

    timer_selection = TIMER_SEL1;
    timer_port = TIMER_1;

  } else if (timer == 2) {

    timer_selection = TIMER_SEL2;
    timer_port = TIMER_2;

  } else {

    printf("INVALID TIMER VALUE\n");
    return 1;
    
  }

  if (timer_get_conf(timer, &st) != F_OK) {
    printf("AN ERROR OCURRED WHEN GETTING THE CONFIGURATION OF THE TIMER");
    return 1;
  }
  
  uint8_t ctrl_wrd = (timer_selection | TIMER_LSB_MSB | ((BIT(3) | BIT(2) | BIT(1) | BIT(0)) & st));

  if(sys_outb(TIMER_CTRL, (uint32_t)ctrl_wrd) != F_OK) {
      printf("AN ERROR OCURRED WHEN WRITTING THE CONTROL WORD TO THE TIMER CONTROL PORT\n");
      return 1;
  }

  uint16_t div = TIMER_FREQ / freq;

  uint8_t msb_div = 0;
  uint8_t lsb_div = 0;

  util_get_MSB(div, &msb_div);
  util_get_LSB(div, &lsb_div);


  if(sys_outb(timer_port, lsb_div) != F_OK) {
      printf("AN ERROR OCURRED WHEN WRITTING LSB OF DIV TO THE TIMER\n");
      return 1;
  }

  if(sys_outb(timer_port, msb_div) != F_OK) {
      printf("AN ERROR OCURRED WHEN WRITTING MSB OF DIV TO THE TIMER\n");
      return 1;
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hook_id);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != F_OK) {
    printf("FAILED TO SET IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

int (timer_unsubscribe_int)() {
  
  if (sys_irqrmpolicy(&hook_id) != F_OK) {
    printf("FAILED TO REMOVE IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

void (timer_int_handler)() {
  timer0_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, rb_cmd) != F_OK) {
    printf("AN ERROR OCURRED WHEN WRITTING THE READ_BACK COMMAND TO THE TIMER CONTROL PORT\n");
    return 1;
  }

  int port = 0;

  if (timer == 0) {
    port = TIMER_0;
  } else if (timer == 1) {
    port = TIMER_1;
  } else if (timer == 2) {
    port = TIMER_2;
  } else {
    printf("INVALID TIMER VALUE");
  }
  
  if (util_sys_inb(port, st) != F_OK) {
    printf("AN ERROR OCURRED WHEN READING THE STATUS BYTE FROM THE DESIRED TIMER\n");
    return 1;
  }
  
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val config;

  if (field == tsf_all) {
    config.byte = st;
  } else if (field == tsf_initial) {
    config.in_mode = (st & TIMER_LSB_MSB) >> 4;
  } else if (field == tsf_mode) {
    config.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;

    //make values 110 and 111, equal to 2 and 3 respectively
    if (config.count_mode > 5) {
      config.count_mode = config.count_mode ^ BIT(2);
    }
  } else if (field == tsf_base) {
    config.bcd = st & TIMER_BCD;
  } else {
    printf("THE STATUS FIELD RECEIVED IS INCORRECT\n");
    return 1;
  }

  if (timer_print_config(timer, field, config) != F_OK) {
    printf ("AN ERROR OCURRED WHEN TRYING TO PRINT THE CONFIGURATION\n");
    return 1;
  }

  return 0;
}

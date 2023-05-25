#include "timer.h"

uint32_t timer0_cnt = 0;
int timer0_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st = 0;

  if(freq < 19 || freq > TIMER_MAX_FREQ) {
    printf("ERROR, FRQUENCY OUT OF VALID RANGE\n");
    return 1;
  }

  if (timer_get_conf(timer, &st) != F_OK) {
    printf("AN ERROR OCURRED WHEN GETTING THE CONFIGURATION OF THE TIMER");
    return 1;
  }
  
  uint8_t ctrl_wrd = (TIMER_SEL_N(timer) | TIMER_LSB_MSB | (st & 0xF));

  if(sys_outb(TIMER_CTRL, (uint32_t)ctrl_wrd) != F_OK) {
      printf("AN ERROR OCURRED WHEN WRITTING THE CONTROL WORD TO THE TIMER CONTROL PORT\n");
      return 1;
  }

  uint16_t div = TIMER_MAX_FREQ / freq;

  uint8_t lsb_div = 0;
  uint8_t msb_div = 0;

  util_get_LSB(div, &lsb_div);
  util_get_MSB(div, &msb_div);

  if(sys_outb(TIMER_N(timer), lsb_div) != F_OK) {
      printf("AN ERROR OCURRED WHEN WRITTING LSB OF DIV TO THE TIMER\n");
      return 1;
  }

  if(sys_outb(TIMER_N(timer), msb_div) != F_OK) {
      printf("AN ERROR OCURRED WHEN WRITTING MSB OF DIV TO THE TIMER\n");
      return 1;
  }

  return 0;
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

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(timer0_hook_id);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer0_hook_id) != OK) {
    printf("FAILED TO SET TIMER0'S IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer0_hook_id) != OK) {
    printf("FAILED TO REMOVE TIMER0'S IRQ POLICY\n");
    return 1;
  }
  
  return 0;
}

void (timer_int_handler)() {
  timer0_cnt++;
}

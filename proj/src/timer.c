#include "timer.h"

uint32_t timer0_interrupt_cnt = 0;
int timer0_hook_id = 0;

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
  timer0_interrupt_cnt++;
}

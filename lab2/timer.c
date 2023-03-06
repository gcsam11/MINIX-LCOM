#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "utils.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
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
  
  if (utils_sys_inb(port, st) != F_OK) {
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
    config.count_mode = (st & (BIT(1) | BIT(2) | BIT(3)) >> 1);

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

#ifndef PROJ_RTC_H
#define PROJ_RTC_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "rtc_macros.h"

typedef struct {
  uint8_t day;
  uint8_t month;
  uint8_t year;
} Date;

void (rtc_ih)();

int (rtc_subscribe_int)(uint32_t *bit_no);

int (rtc_unsubscribe_int)();

int (rtc_is_binary)();

uint8_t (to_binary)(uint8_t bcd_number);

int (rtc_write_addr)(uint8_t addr);

int (rtc_read_data)(uint8_t *data);

int (rtc_write_data)(uint8_t data);

Date (rtc_read_date) ();

#endif

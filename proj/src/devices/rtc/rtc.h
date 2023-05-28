#ifndef PROJ_RTC_H
#define PROJ_RTC_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "rtc_macros.h"

typedef struct {
    uint32_t seconds;
    uint32_t minutes;
    uint32_t hours;
    uint32_t day;
    uint32_t month;
    uint32_t year;
} date_time_t;

typedef struct {
  uint16_t day;
  uint16_t month;
  uint16_t year;
} Date;

void (wait_until_valid_rtc)();

/*
void (rtc_ih)();

int (rtc_subscribe_int)(uint32_t *bit_no);

int (rtc_unsubscribe_int)();

int (rtc_read_reg)(uint32_t addr, uint32_t *data);

int (rtc_write_reg)(uint32_t addr, uint32_t data);
*/

int(rtc_write_addr)(uint8_t addr);

int(rtc_read_data)(uint8_t *data);

int(rtc_write_data)(uint8_t data);

Date (rtc_read_date) ();

#endif

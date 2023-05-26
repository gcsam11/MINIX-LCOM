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

void (wait_until_valid_rtc)();

void (rtc_ih)();

int (rtc_subscribe_int)(uint32_t *bit_no);

int (rtc_unsubscribe_int)();

int (rtc_read_reg)(uint32_t addr, uint32_t *data);

int (rtc_write_reg)(uint32_t addr, uint32_t data);

#endif

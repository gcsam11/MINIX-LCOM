#include "rtc.h"

void (wait_until_valid_rtc)() {
    uint8_t data = 0;

    do {
        sys_outb(RTC_ADDR_REG, RTC_REG_A);
        util_sys_inb(RTC_DATA_REG, &data);
    } while ((data & UIP) != 0);    
}

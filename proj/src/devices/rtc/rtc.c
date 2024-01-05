#include "rtc.h"

int rtc_is_binary() {
  uint8_t result;
  if (rtc_write_addr(RTC_REG_B)) return 1;
  if (rtc_read_data(&result)) return 1;

	return result & DM;
}

uint8_t to_binary(uint8_t bcd_number) {
    return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

int(rtc_write_addr)(uint8_t addr) {
  return sys_outb(RTC_ADDR_REG, addr);
}

int(rtc_read_data)(uint8_t *data) {
  return util_sys_inb(RTC_DATA_REG, data);
}

int(rtc_write_data)(uint8_t data) {
  return sys_outb(RTC_DATA_REG, data);
}

Date (rtc_read_date)() {
  uint8_t reg = 0;

  do {
    rtc_write_addr(RTC_REG_A);
    rtc_read_data(&reg);
  } while (reg & UIP);

  rtc_write_addr(RTC_REG_B);
  rtc_read_data(&reg);

  reg |= DM;

  rtc_write_addr(RTC_REG_B);
  rtc_write_data(reg);

  uint8_t time[3] = {0, 0, 0};
  uint8_t regs[3] = {RTC_MONTHDAY, RTC_MONTH, RTC_YEAR};

  for (int i = 0; i < 3; i++) {
    rtc_write_addr(regs[i]);
    rtc_read_data(&time[i]);
  }

  Date date = {time[0], time[1], time[2]};

  return date;
}

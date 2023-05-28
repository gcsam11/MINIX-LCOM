#include "rtc.h"

static int rtc_hook_id;

void (rtc_ih)() {
    uint32_t regA;

    sys_outb(RTC_ADDR_REG, RTC_REG_C);
    sys_inb(RTC_DATA_REG, &regA);
}

int (rtc_subscribe_int)(uint32_t *bit_no) {
    if (!bit_no) {return 1;}

    rtc_hook_id = RTC_IRQ;

    *bit_no = BIT(RTC_IRQ);

    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id)) {return 1;}

    return 0;
}

int (rtc_unsubscribe_int)() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

int rtc_is_binary() {
    uint8_t result;
    if (rtc_write_addr(11)) return 1;
    if (rtc_read_data(&result)) return 1;
	return result & BIT(2);
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
  uint8_t reg;
  rtc_write_addr(RTC_REG_B);
  rtc_read_data(&reg);

  reg |= DM;

  rtc_write_addr(RTC_REG_B);
  rtc_write_data(reg);

  rtc_write_addr(RTC_REG_A);
  rtc_read_data(&reg);

  while (!reg & UIP) {
    rtc_write_addr(RTC_REG_A);
    rtc_read_data(&reg);
  }

  tickdelay(micros_to_ticks(244));

  uint8_t time[3] = {0, 0, 0};
  uint8_t regs[3] = {RTC_MONTHDAY, RTC_MONTH, RTC_YEAR};

  for (int i = 0; i < 3; i++) {
    rtc_write_addr(regs[i]);
    rtc_read_data(&time[i]);
  }

  Date date = {time[0], time[1], time[2]};

  date.day = rtc_is_binary() ? time[0] : to_binary(time[0]);
  date.month = rtc_is_binary() ? time[1] : to_binary(time[1]);
  date.year = rtc_is_binary() ? time[2] : to_binary(time[2]);

  return date;
}

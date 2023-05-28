#include "rtc.h"

//static int rtc_hook_id;

void (wait_until_valid_rtc)() {
    uint8_t data = 0;

    do {
        sys_outb(RTC_ADDR_REG, RTC_REG_A);
        util_sys_inb(RTC_DATA_REG, &data);
    } while ((data & UIP) != 0);    
}

/*
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

int (rtc_read_reg)(uint32_t addr, uint32_t* data) {
    if (sys_outb(RTC_ADDR_REG, addr)) {return 1;}

    if (sys_inb(RTC_DATA_REG, data)) {return 1;}

    return 0;
}

int (rtc_write_reg)(uint32_t addr, uint32_t data) {
    if (sys_outb(RTC_ADDR_REG, addr)) {return 1;}

    if (sys_outb(RTC_DATA_REG, data)) {return 1;}

    return 0;
}

int (bcd_to_dec)(uint16_t bcd) {
    unsigned short MSB, LSB;
    MSB = (bcd >> 4);
    LSB = (bcd & 0x0F);

    if (!MSB) {return LSB;}
    else if (MSB < 8) {return 10 * MSB + LSB;}
    else if (MSB == 8) {return 12 + LSB;}
    else {return 22 + LSB;}
}

uint16_t (dec_to_bcd)(uint16_t dec) {
    return (((dec / 10) << 4) + (dec % 10));
}

date_time_t get_date() {
    date_time_t date;
    uint32_t data;

    rtc_read_reg(RTC_REG_A, &data);

    if (data & UIP) {tickdelay(micros_to_ticks(UIP));}

    rtc_read_reg(RTC_MONTHDAY, &data);
    date.day = bcd_to_dec(data);

    rtc_read_reg(RTC_MONTH, &data);
    date.month = bcd_to_dec(data);

    rtc_read_reg(RTC_YEAR, &data);
    date.year = bcd_to_dec(data);

    return date;
}

date_time_t get_time() {
    date_time_t time;
    uint32_t data;

    rtc_read_reg(RTC_REG_A, &data);

    if (data & UIP) {tickdelay(micros_to_ticks(UIP));}

    rtc_read_reg(RTC_SEC, &data);
    time.seconds = bcd_to_dec(data);

    rtc_read_reg(RTC_MIN, &data);
    time.minutes = bcd_to_dec(data);

    rtc_read_reg(RTC_HOUR, &data);
    time.hours = bcd_to_dec(data);

    return time;
}
*/


int(rtc_write_addr)(uint8_t addr) {
  return sys_outb(RTC_ADDR_REG, addr);
}

int(rtc_read_data)(uint8_t *data) {
  return util_sys_inb(RTC_DATA_REG, data);
}

int(rtc_write_data)(uint8_t data) {
  return sys_outb(RTC_DATA_REG, data);
}

Date (rtc_read_date) () {

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
  return date;
}

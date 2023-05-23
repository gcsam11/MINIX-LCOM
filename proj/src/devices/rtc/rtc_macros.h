#ifndef PROJ_RTC_MACROS_H
#define PROJ_RTC_MACROS_H

#include <lcom/lcf.h>

#define RTC_IRQ        8

#define RTC_ADDR_REG   0X70
#define RTC_DATA_REG   0X71

#define RTC_SEC        0X00
#define RTC_SEC_ALARM  0X01
#define RTC_MIN        0X02
#define RTC_MIN_ALARM  0X03
#define RTC_HOUR       0X04
#define RTC_HOUR_ALARM 0X05
#define RTC_WEEKDAY    0X06
#define RTC_MONTHDAY   0X07
#define RTC_MONTH      0X08
#define RTC_YEAR       0X09

#define RTC_REG_A      0X0A
#define RTC_REG_B      0X0B
#define RTC_REG_C      0X0C
#define RTC_REG_D      0X0D

#define UIP            BIT(7)
#define RS3            BIT(3)
#define RS2            BIT(2)
#define RS1            BIT(1)
#define RS0            BIT(0)

#define SET            BIT(7)
#define PIE            BIT(6)
#define AIE            BIT(5)
#define UIE            BIT(4)
#define SQWE           BIT(3)
#define DM             BIT(2)
#define RANGE_24_12    BIT(1)

#define IRQF           BIT(7)
#define PF             BIT(6)
#define AF             BIT(5)
#define UF             BIT(4)

#define VRT            BIT(7)

#endif

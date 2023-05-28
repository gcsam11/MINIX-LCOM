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

/**
 * @brief RTC's interrupt handler
*/
void (rtc_ih)();

/**
 * @brief This function subscribes RTC interrupts
 * @return Returns 0 if case of success, 1 if otherwise
*/
int (rtc_subscribe_int)(uint32_t *bit_no);

/**
 * @brief This function unsubscribes RTC interrupts
 * @return Returns 0 if case of success, 1 if otherwise
*/
int (rtc_unsubscribe_int)();

/**
 * @brief This function checks if the rtc is in binary
 * @returns Returns true if binary, false if not
*/
int (rtc_is_binary)();

/**
 * @brief converts BCD number to binary
 * @param bcd_number Number in BCD form
 * @return Returns number converted to binary
*/
uint8_t (to_binary)(uint8_t bcd_number);

/**
 * @brief This function writes to address
 * @param addr 
 * @return Returns 0 if case of success, 1 if otherwise
*/
int (rtc_write_addr)(uint8_t addr);

/**
 * @brief This function reads data from port
 * @return Returns 0 if case of success, 1 if otherwise
*/
int (rtc_read_data)(uint8_t *data);

/**
 * @brief This function writes data to port
 * @return Returns 0 if case of success, 1 if otherwise
*/
int (rtc_write_data)(uint8_t data);

/**
 * @brief This function reads the current date 
 * @return Returns the date
*/
Date (rtc_read_date) ();

#endif

#ifndef PROJ_KEYBOARD_H
#define PROJ_KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "kbc_macros.h"

/**
* @brief This function subscribes keyboard interrupts with policies REENABLE and EXCLUSIVE
* @param bit_no All zeros except for the bit position of the keyboard's hook id
* @return Return 0 if case of success, 1 if otherwise
*/
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
* @brief This function unsubscribes keyboard interrupts
* @return Return 0 if case of success, 1 if otherwise
*/
int (kbd_unsubscribe_int)();

/**
* @brief This function reads the status of the KBC
* @param data The current status of the KBC
* @return Return 0 if case of success, 1 if otherwise
*/
int(kbd_read_data)(uint8_t *data);

/**
* @brief Keyboard's interrupt handler
*/
void (kbd_int_handler)();

/**
* @brief This function writes a command to the specified port
* @param kbc_cmd_reg Port to write to
* @param cmd The command byte to write
* @return Return 0 if case of success, 1 if otherwise
*/
int(kbc_issue_cmd)(int kbc_cmd_reg, uint8_t cmd);

#endif

#ifndef PROJ_MOUSE_H
#define PROJ_MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "../keyboard/keyboard.h"
#include "../keyboard/kbc_macros.h"

/**
 * @brief This function uses irq_setpolicy to configure mouse interrupts
 * @param bit_no All zeros except for the bit position of the timer's hook id
 * @return Return 0 if case of success, 1 if otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief This function unsubscribes mouse interrupts
 * @return Return 0 if case of success, 1 if otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief This function enables Mouse data reporting
 * @return Return 0 if case of success, 1 if otherwise
*/
int (my_mouse_enable_data_reporting)();

/**
 * @brief This function disables Mouse data reporting
 * @return Return 0 if case of success, 1 if otherwise
*/
int (my_mouse_disable_data_reporting)();

/**
 * @brief Mouse's interrupt handler
 */
void (mouse_int_handler)();

/**
 * @brief This function reads data from the port
 * @return Return 0 if case of success, 1 if otherwise
*/
int(mouse_read_data)(uint8_t *data);

/**
 * @brief  This function issues the control word to the keyboard, using kbc_issue_cmd()
 * @return Return 0 if case of success, 1 if otherwise
*/
int(mouse_send_cmd)(uint8_t cmd);

/**
 * @brief When mouse packet array has stored all the necessary bytes this functions interprets it and updates mouse information
 * @return Return 0 if case of success, 1 if otherwise
 */
void (mouse_parse_packet)(struct packet* mouse_pp);

#endif

#ifndef PROJ_TIMER_H
#define PROJ_TIMER_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "timer_macros.h"

/**
* @brief This function uses irq_setpolicy to configure timer interrupts
* @param bit_no
* @return Return 0 in case of success, 1 if otherwise
*/
int (timer_subscribe_int)(uint8_t *bit_no);

/**
* @brief This function unsubscribes timer interrupts
* @return Return 0 in case of success, 1 if otherwise
*/
int (timer_unsubscribe_int)();

/**
 * @brief This function sets the frame rate of the program
 * @return Return 0 in case of success, 1 if otherwise
*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
* @brief this function reads from the timer given by input its status
* @param timer specifies the timer used
* @param st variable where the information read will be stored 
* @return Return 0 in case of sucess, 1 otherwise
*/
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
* @brief Timer's interrupt handler, increments the counter
*/
void (timer_int_handler)();

#endif

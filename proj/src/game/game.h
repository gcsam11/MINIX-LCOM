#ifndef PROJ_GAME_H
#define PROJ_GAME_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "events/event_handlers.h"
#include "game_aux/game_aux.h"

/**
 * @brief Subscribes interrupts, starts video mode, sets game state to MENU
*/
void (game_init)();

/**
 * @brief Runs main interrupts loop
*/
void (game_run)();

/**
 * @brief Clears the screen memory, exits video mode, unsubscribes interrupts
*/
void (game_exit)();

#endif

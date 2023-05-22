#ifndef PROJ_KEYBOARD_MACROS_H
#define PROJ_KEYBOARD_MACROS_H

#include <lcom/lcf.h>

#define KBD_IRQ              1

#define ACTIVATE_BRKCODE     BIT(7)

#define FIRSTBYTE_2BYTECODE  0xE0
#define ESC_MAKECODE         0x01
#define ESC_BREAKCODE        0x81
#define W_MAKECODE           0x11
#define W_BREAKCODE          0x91
#define A_MAKECODE           0x1E
#define A_BREAKCODE          0x9E
#define S_MAKECODE           0x1F
#define S_BREAKCODE          0x9F
#define D_MAKECODE           0x20
#define D_BREAKCODE          0xA0

#endif

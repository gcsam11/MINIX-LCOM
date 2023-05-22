#ifndef PROJ_MOUSE_MACROS_H
#define PROJ_MOUSE_MACROS_H

#include <lcom/lcf.h>

#define MOUSE_IRQ            12

#define MOUSE_RESET          0xFF
#define MOUSE_RESEND         0xFE
#define MOUSE_SET_DEFAULTS   0xF6
#define MOUSE_DISBL_DT_RPORT 0xF5
#define MOUSE_ENBL_DT_RPORT  0xF4
#define MOUSE_SET_SMPL_RT    0xF3
#define MOUSE_SET_RMT_MD     0xF0
#define MOUSE_READ_DT        0xEB
#define MOUSE_SET_STRM_MD    0xEA
#define MOUSE_ST_REQ         0xE9
#define MOUSE_SET_RES        0xE8
#define MOUSE_SCL_21         0xE7
#define MOUSE_SCL_11         0xE6

#define MOUSE_ACK            0xFA
#define MOUSE_NACK           0xFE
#define MOUSE_ERROR          0xFC

#define MOUSE_PCKT_YOVFL     BIT(7)
#define MOUSE_PCKT_XOVFL     BIT(6)
#define MOUSE_PCKT_MSBY_DEL  BIT(5)
#define MOUSE_PCKT_MSBX_DEL  BIT(4)
#define MOUSE_PCKT_3         BIT(3)
#define MOUSE_PCKT_MB        BIT(2)
#define MOUSE_PCKT_RB        BIT(1)
#define MOUSE_PCKT_LB        BIT(0)

#endif

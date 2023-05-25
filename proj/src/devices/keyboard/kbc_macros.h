#ifndef PROJ_KBC_MACROS_H
#define PROJ_KBC_MACROS_H

#include <lcom/lcf.h>

#define DELAY_US             20000

#define KBD_IRQ              1
#define MOUSE_IRQ            12

#define KBC_ST_REG           0X64
#define KBC_IN_BUF_CMDS      0X64
#define KBC_IN_BUF_ARGS      0X60
#define KBC_OUT_BUF          0X60

#define KBC_READ_CMD_BT      0x20
#define KBC_WRITE_CMD_BT     0x60
#define KBC_SELF_CHECK       0xAA
#define KBC_CHECK_KBD_IFC    0xAB
#define KBC_DISBL_KBD_IFC    0XAD
#define KBC_ENBL_KBD_IFC     0XAE

#define KBC_STREG_PARITY     BIT(7)
#define KBC_STREG_TIMEOUT    BIT(6)
#define KBC_STREG_AUX        BIT(5)
#define KBC_STREG_INH        BIT(4)
#define KBC_STREG_A2         BIT(3)
#define KBC_STREG_SYS        BIT(2)
#define KBC_STREG_IBF        BIT(1)
#define KBC_STREG_OBF        BIT(0)

#define KBC_DIS2             BIT(5)
#define KBC_DIS              BIT(4)
#define KBC_INT2             BIT(1)
#define KBC_INT              BIT(0)

#define KBC_DISBL_MOUSE      0xA7
#define KBC_ENBL_MOUSE       0xA8
#define KBC_CHECK_MOUSE_IFC  0xA9
#define KBC_WRITE_BT_MOUSE   0xD4

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
#define ONE_MAKECODE         0x02
#define TWO_MAKECODE         0x03

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

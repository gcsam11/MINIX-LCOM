#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US 20000

#define KBC_IRQ 1

#define ESC_BREAK 0x81
#define KBC_2BYTECODE_FIRST 0xE0

#define KBC_ST_REG 0X64
#define KBC_IN_BUF_CMDS 0X64
#define KBC_IN_BUF_ARGS 0X60
#define KBC_OUT_BUF 0X60

#define KBC_READ_CMD_BT 0x20
#define KBC_WRITE_CMD_BT 0x60
#define KBC_SELF_CHECK 0xAA
#define KBC_CHECK_KBD_IFC 0xAB
#define KBC_DISBL_KBD_IFC 0XAD
#define KBC_ENBL_KBD_IFC 0XAE

#define KBC_ACTIVATE_BRKCODE BIT(7)

#define KBC_STREG_PARITY BIT(7)
#define KBC_STREG_TIMEOUT BIT(6)
#define KBC_STREG_AUX BIT(5)
#define KBC_STREG_INH BIT(4)
#define KBC_STREG_A2 BIT(3)
#define KBC_STREG_SYS BIT(2)
#define KBC_STREG_IBF BIT(1)
#define KBC_STREG_OBF BIT(0)

#define KBC_DIS2 BIT(5)
#define KBC_DIS BIT(4)
#define KBC_INT2 BIT(1)
#define KBC_INT BIT(0)

#endif

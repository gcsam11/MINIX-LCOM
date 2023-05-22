#ifndef PROJ_KBC_H
#define PROJ_KBC_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "kbc_macros.h"

int(kbc_issue_cmd)(int kbc_cmd_reg, uint8_t cmd);

#endif

#ifndef PROJ_H
#define PROJ_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <time.h>

#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "video_card.h"
#include "sprite.h"
#include "pixelmaps.h"

int main(int argc, char *argv[]);

int(proj_main_loop)(int argc, char *argv[]);

#endif

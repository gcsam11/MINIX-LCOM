# name of the program (Minix service)
PROG = proj

.PATH: devices devices/keyboard devices/mouse devices/rtc devices/timer devices/video_card game game/game_aux game/events sprites sprites/pixelmaps

# source code files to be compiled
SRCS = proj.c game.c event_handlers.c game_aux.c sprite.c keyboard.c mouse.c rtc.c timer.c video_card.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIBLM} ${LIBXPM}
LDADD += -llcf -llm -lxpm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

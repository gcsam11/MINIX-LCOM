#include "game.h"

extern uint8_t kbd_irq_set, timer_irq_set, mouse_irq_set;

extern bool running;

void (game_init)() {
    subscribe_interrupts();

    vg_init(0x118);

    set_game_state(MENU);
}

void (game_run)() {
    int r, ipc_status;
    message msg;

    running = true;
    while(running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts &kbd_irq_set) {
                        kbd_int_handler();
                        kbd_event_handler();
                    }

                    if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
                        mouse_int_handler();
                        mouse_event_handler();
                    }

                    if (msg.m_notify.interrupts &timer_irq_set) {
                        timer_int_handler();
                        timer_event_handler();
                    }

                    break;

                default:

                    break;
            }
        }
    }
}

void (game_exit)() {
    clear_game_state(MENU);

    vg_exit();

    unsubscribe_interrupts();
}

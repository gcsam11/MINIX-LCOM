#include "game.h"

extern uint8_t kbd_irq_set, timer_irq_set, mouse_irq_set;

extern uint32_t timer0_interrupt_cnt;
extern uint8_t scancode;
extern struct packet pp;

enum game_state_t game_state;

uint8_t update_rate = FREQUENCY / FRAME_RATE;

bool HERO_MOVING = false, MOUSE_MOVED = false, W_ISPRESSED, A_ISPRESSED, S_ISPRESSED, D_ISPRESSED;

Sprite* planthero;
Sprite* mouse;

void (set_sprites)() {
    vg_set_background(background_xpm); //function to set what background the video card will draw (directly an xpm, because it doesn´t need position or speed, so it doesn´t need to be a sprite), to actually draw use (vg_draw_background)
    mouse = create_sprite(mouse_xpm, 200, 200, 0, 0);
    planthero = create_sprite(planthero_xpm, 0, 0, 0, 0);
}

void (render)(Sprite** sprites) {
    vg_clear_frame();

    vg_draw_background();

    int sprites_len = sizeof(sprites)/sizeof(sprites[0]);

    for (int i = 0; i < sprites_len; i++) {
        draw_sprite(sprites[i]);
    }

    page_flip();
}

void (game_init)() {
    subscribe_interrupts();

    vg_init(0x118);
    set_sprites();

    Sprite* sprites[] = {mouse};
    render(sprites);

    game_state = MENU;
}

void (game_run)() {
    int r, ipc_status;
    message msg;

    while(scancode != ESC_MAKECODE) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts &kbd_irq_set) {
                        kbd_ih();
                        keyboard_event_handler();
                    }

                    if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
                        mouse_ih();
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
    unsubscribe_interrupts();

    destroy_sprite(&planthero);

    vg_exit();
}

void (keyboard_event_handler)() {
    if (game_state == MENU) {
        
    }

    if (game_state == GAMEPLAY) {
        switch (scancode) {
            case W_MAKECODE:
                W_ISPRESSED = true;
                set_sprite_vy(planthero, -240);

                break;

            case A_MAKECODE:
                A_ISPRESSED = true;
                set_sprite_vx(planthero, -240);

                break;

            case S_MAKECODE:
                S_ISPRESSED = true;
                set_sprite_vy(planthero, 240);

                break;

            case D_MAKECODE:
                D_ISPRESSED = true;
                set_sprite_vx(planthero, 240);

                break;

            case (W_BREAKCODE):
                W_ISPRESSED = false;

                if (S_ISPRESSED) {
                    set_sprite_vy(planthero, 240);
                } else {
                    set_sprite_vy(planthero, 0);
                }

                break;

            case (A_BREAKCODE):
                A_ISPRESSED = false;

                if (D_ISPRESSED) {
                    set_sprite_vx(planthero, 240);
                } else {
                    set_sprite_vx(planthero, 0);
                }

                break;

            case (S_BREAKCODE):
                S_ISPRESSED = false;

                if (W_ISPRESSED) {
                    set_sprite_vy(planthero, -240);
                } else {
                    set_sprite_vy(planthero, 0);
                }

                break;

            case (D_BREAKCODE):
                D_ISPRESSED = false;

                if (A_ISPRESSED) {
                    set_sprite_vx(planthero, -240);
                } else {
                    set_sprite_vx(planthero, 0);
                }

                break;
            
            default:
                break;
        }

        HERO_MOVING = W_ISPRESSED || A_ISPRESSED || S_ISPRESSED || D_ISPRESSED;
    }
}

void (mouse_event_handler)() {
    struct mouse_ev* event = mouse_detect_event(&pp);

    if (game_state == MENU) {
        if (event->type == MOUSE_MOV) {
            set_sprite_vx(mouse, event->delta_x * FRAME_RATE);
            set_sprite_vy(mouse, -event->delta_y * FRAME_RATE);
            MOUSE_MOVED = true;
        }
        if (event->type == LB_PRESSED) {
            Sprite* sprites[] = {planthero};
            render(sprites);
            game_state = GAMEPLAY;
        }
    }

    if (game_state == GAMEPLAY) {

    }
}

void (timer_event_handler)() {
    if (game_state == MENU) {
        if (timer0_interrupt_cnt % update_rate == 0) {
            if (MOUSE_MOVED) {
                update_sprite_position(mouse, FIXED_TIMESTEP);

                Sprite* sprites[] = {mouse};
                render(sprites);

                MOUSE_MOVED = false;
            }
        }
    }

    if (game_state == GAMEPLAY) {
        if (timer0_interrupt_cnt % update_rate == 0) {
            if (HERO_MOVING) {
                update_sprite_position(planthero, FIXED_TIMESTEP);

                Sprite* sprites[] = {planthero};
                render(sprites);
            }
        }
    }
}

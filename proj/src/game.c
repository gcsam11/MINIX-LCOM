#include "game.h"

extern uint8_t kbd_irq_set, timer_irq_set, mouse_irq_set;

extern uint8_t scancode;
extern struct packet mouse_pp;
extern uint32_t timer0_cnt;

enum game_state_t game_state;

bool running;

uint8_t update_rate = FREQUENCY / FRAME_RATE;

bool HERO_MOVED = false, MOUSE_MOVED = false, W_ISPRESSED, A_ISPRESSED, S_ISPRESSED, D_ISPRESSED;

Sprite* planthero;
Sprite* zombie;
Sprite* mouse;
//Sprite* play_button;
//Sprite* date_button;
//Sprite* quit_button;

void (game_init)() {
    subscribe_interrupts();

    vg_init(0x118);

    planthero = create_sprite(planthero_xpm, 0, 0, 0, 0);
    zombie = create_sprite(zombie_xpm, 400, 350, 0, 0);
    mouse = create_sprite(mouse_xpm, 518, 384, 0, 0);
    //play_button = create_sprite(play_white_xpm, 450, 150, 0, 0);
    //date_button = create_sprite(date_white_xpm, 450, 250, 0, 0);
    //quit_button = create_sprite(quit_white_xpm, 450, 350, 0, 0);

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
    unsubscribe_interrupts();

    destroy_sprite(&planthero);
    destroy_sprite(&zombie);
    destroy_sprite(&mouse);
    //destroy_sprite(&play_button);
    //destroy_sprite(&date_button);
    //destroy_sprite(&quit_button);

    vg_exit();
}

void (render_frame)(Sprite** sprites) {
    vg_clear_frame();

    vg_draw_background();

    size_t num_sprites = sizeof(sprites)/sizeof(Sprite*);
    for (size_t i = 0; i <= num_sprites; i++) {
        draw_sprite(sprites[i]);
    }

    page_flip();
}

void (set_game_state)(enum game_state_t state) {
    switch (state) {
        case MENU: {
            vg_set_background(backgroundMainMenu_xpm);
 
            set_sprite_x(mouse, 518);
            set_sprite_y(mouse, 384);

            Sprite* sprites[] = {mouse};
            //Sprite* sprites[] = {mouse, play_button, date_button, quit_button};
            render_frame(sprites);
            
            break;
        }
        case GAMEPLAY: {
            vg_set_background(background_gameplay_xpm);

            set_sprite_vx(planthero, 0);
            set_sprite_vy(planthero, 0);
            set_sprite_x(planthero, 0);
            set_sprite_y(planthero, 0);
            set_sprite_x(zombie, 400);
            set_sprite_y(zombie, 350);

            Sprite* sprites[] = {planthero, zombie};
            render_frame(sprites);

            break;
        }
        case DATE: {
            break;
        }
        
        default:
            break;
    }
    game_state = state;
}

void kbd_event_handler() {
    switch (scancode) {
        case ESC_MAKECODE:
            if (game_state == MENU) {
                running = false;
            }
            else if (game_state == GAMEPLAY) {
                set_game_state(MENU);
            }
            break;
        case W_MAKECODE:
            if (game_state == GAMEPLAY) {
                W_ISPRESSED = true;
                set_sprite_vy(planthero, -5);
            }
            break;

        case A_MAKECODE:
            if (game_state == GAMEPLAY) {
                A_ISPRESSED = true;
                set_sprite_vx(planthero, -5);
            }
            break;

        case S_MAKECODE:
            if (game_state == GAMEPLAY) {
                S_ISPRESSED = true;
                set_sprite_vy(planthero, 5);
            }
            break;

        case D_MAKECODE:
            if (game_state == GAMEPLAY) {
                D_ISPRESSED = true;
                set_sprite_vx(planthero, 5);
            }
            break;

        case W_BREAKCODE:
            if (game_state == GAMEPLAY) {
                W_ISPRESSED = false;

                if (S_ISPRESSED) {
                    set_sprite_vy(planthero, 5);
                } else {
                    set_sprite_vy(planthero, 0);
                }
            }
            break;

        case A_BREAKCODE:
            if (game_state == GAMEPLAY) {
                A_ISPRESSED = false;

                if (D_ISPRESSED) {
                    set_sprite_vx(planthero, 5);
                } else {
                    set_sprite_vx(planthero, 0);
                }
            }
            break;

        case S_BREAKCODE:
            if (game_state == GAMEPLAY) {
                S_ISPRESSED = false;

                if (W_ISPRESSED) {
                    set_sprite_vy(planthero, -5);
                } else {
                    set_sprite_vy(planthero, 0);
                }
            }
            break;

        case D_BREAKCODE:
            if (game_state == GAMEPLAY) {
                D_ISPRESSED = false;

                if (A_ISPRESSED) {
                    set_sprite_vx(planthero, -5);
                } else {
                    set_sprite_vx(planthero, 0);
                }
            }
            break;

        case ONE_MAKECODE:
            if (game_state == MENU) {
                set_game_state(GAMEPLAY);
            }
            break;
        
        case TWO_MAKECODE:
            if (game_state == MENU) {
                set_game_state(DATE);
            }
            break;

        default:
            break;
    }

    HERO_MOVED = W_ISPRESSED || A_ISPRESSED || S_ISPRESSED || D_ISPRESSED;
}

void mouse_event_handler() {
    struct mouse_ev* event = mouse_detect_event(&mouse_pp);

    if (event->type == MOUSE_MOV) {
        if (game_state == MENU) {
            set_sprite_vx(mouse, event->delta_x);
            set_sprite_vy(mouse, -event->delta_y);
            MOUSE_MOVED = true;
        }
    }

    if (event->type == LB_PRESSED) {
        if (game_state == MENU) {
            set_game_state(GAMEPLAY);
        }
    }
}


void timer_event_handler() {
    if (timer0_cnt % update_rate == 0) {
        if (game_state == MENU) {
            if (MOUSE_MOVED) {
                update_sprite_position(mouse);

                Sprite* sprites[] = {mouse};
                render_frame(sprites);

                MOUSE_MOVED = false;
            }
        }

        if (game_state == GAMEPLAY) {
            if (HERO_MOVED) {
                update_sprite_position(planthero);

                bool collided = check_sprite_collision(planthero, zombie);
                if (collided) {
                    set_game_state(MENU);
                }

                Sprite* sprites[] = {planthero, zombie};
                render_frame(sprites);
            }
        }
    }
}

#include "event_handlers.h"

Sprite* render_sprites[20];

Sprite* mouse;
Sprite* play_button;
Sprite* date_button;
Sprite* quit_button;
Sprite* title;

Sprite* planthero;
Sprite* shots[5];
Sprite* zombies[10];
Sprite* score_sprite[4];

bool W_ISPRESSED, A_ISPRESSED, S_ISPRESSED, D_ISPRESSED;

bool MOUSE_MOVING;

bool WHITE1_SET = true, WHITE2_SET = true, WHITEQUIT_SET = true;

uint8_t shots_fired = 0;

uint8_t zombies_alive;

int zombies_vx = -1;

uint16_t score = 0;

uint8_t update_rate = FREQUENCY / FRAME_RATE;

enum game_state_t game_state;

bool running;

extern uint8_t scancode;
extern struct packet mouse_pp;
extern uint32_t timer0_cnt;

void (kbd_event_handler)() {
    switch (scancode) {
        case ESC_MAKECODE:
            if (game_state == MENU) {
                running = false;
            }
            else if (game_state == GAMEPLAY) {
                clear_game_state(GAMEPLAY);

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
                clear_game_state(MENU);

                set_game_state(GAMEPLAY);
            }
            break;
        
        case TWO_MAKECODE:
            if (game_state == MENU) {
                clear_game_state(MENU);

                set_game_state(DATE);
            }
            break;

        default:
            break;
    }
}

void (mouse_event_handler)() {
    struct mouse_ev* event = mouse_detect_event(&mouse_pp);

    if (event->type == MOUSE_MOV) {
        if (game_state == MENU) {
            set_sprite_vx(mouse, event->delta_x);
            set_sprite_vy(mouse, -event->delta_y);

            MOUSE_MOVING = true;
        }
    }
    else if (event->type == LB_PRESSED) {
        if (game_state == MENU) {
            bool collided_play = check_sprite_collision(mouse, play_button);
            if (collided_play) {
                clear_game_state(MENU);

                set_game_state(GAMEPLAY);

                return;
            }

            bool collided_date = check_sprite_collision(mouse, date_button);
            if (collided_date) {
                clear_game_state(MENU);

                set_game_state(DATE);

                return;
            }

            bool collided_quit = check_sprite_collision(mouse, quit_button);
            if (collided_quit) {
                running = false;

                return;
            } 
        }
        else if (game_state == GAMEPLAY) {
            if (shots_fired < 5) {
                Sprite* energy_ball = create_sprite(energy_ball_xpm, planthero->x + 152, planthero->y + 40, 10, 0);

                for (int i = 0; i < 5; i++) {
                    if (shots[i] == NULL) {
                        shots[i] = energy_ball;
                        render_sprites[1 + i] = energy_ball;
                        break;
                    }
                    
                }

                shots_fired++;
            }
        }
    }
}

void (timer_event_handler)() {
    if (timer0_cnt % update_rate == 0) {
        if (game_state == MENU) {
            if (MOUSE_MOVING) {
                update_sprite_position(mouse);

                bool collided_play = check_sprite_collision(mouse, play_button);
                if (collided_play) {
                    set_sprite_pixelmap(play_button, play_yellow_xpm);
                    WHITE1_SET = false;
                } else {
                    if (!WHITE1_SET) {
                        set_sprite_pixelmap(play_button, play_white_xpm);
                        WHITE1_SET = true;
                    }
                }

                bool collided_date = check_sprite_collision(mouse, date_button);
                if (collided_date) {
                    set_sprite_pixelmap(date_button, date_yellow_xpm);
                    WHITE2_SET = false;
                } else {
                    if (!WHITE2_SET) {
                        set_sprite_pixelmap(date_button, date_white_xpm);
                        WHITE2_SET = true;
                    }
                }
                        
                bool collided_quit = check_sprite_collision(mouse, quit_button);
                if (collided_quit) {
                    set_sprite_pixelmap(quit_button, quit_yellow_xpm);
                    WHITEQUIT_SET = false;
                } else {
                    if (!WHITEQUIT_SET) {
                        set_sprite_pixelmap(quit_button, quit_white_xpm);
                        WHITEQUIT_SET = true;
                    }
                }

                render_frame();

                MOUSE_MOVING = false;
            }
        }
        else if (game_state == GAMEPLAY) {
            update_sprite_position(planthero);

            update_zombies();

            if (check_hero_zombies_collisions() == true) {
                clear_game_state(GAMEPLAY);

                set_game_state(MENU);

                return;
            }

            update_shots();

            manage_zombies_shot();

            if (zombies_alive == 0) {
                create_zombie_hord();
                zombies_vx *= 2;
            }

            manage_shots_at_edge();

            render_frame();
        }
    }
}

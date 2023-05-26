#include "game_aux.h"

extern Sprite* render_sprites[20];

extern Sprite* mouse;
extern Sprite* play_button;
extern Sprite* date_button;
extern Sprite* quit_button;

extern Sprite* planthero;
extern Sprite* shots[5];
extern Sprite* zombies[10];

extern bool W_ISPRESSED, A_ISPRESSED, S_ISPRESSED, D_ISPRESSED;

extern uint8_t shots_fired;

extern enum game_state_t game_state;

uint8_t kbd_irq_set, timer_irq_set, mouse_irq_set;

int (subscribe_interrupts)() {

    if(timer_subscribe_int(&timer_irq_set) != OK) {
        printf("FAILED TO SUBSCRIBE KEYBOARD\n");
        return 1;
    }

    if (kbd_subscribe_int(&kbd_irq_set) != OK) {
        printf("FAILED TO SUBSCRIBE KEYBOARD\n");
        return 1;
    }

    if (my_mouse_enable_data_reporting() != OK) {
        printf("ERROR WHILE ENABLING MOUSE DATA REPORTING\n");
        return 1;
    }

    if (mouse_subscribe_int(&mouse_irq_set) != OK) {
        printf("FAILED TO SUBSCRIBE MOUSE\n");
        return 1;
    }

    return 0;
}

int (unsubscribe_interrupts)() {

    if (mouse_unsubscribe_int() != OK) {
        printf("FAILED TO UNSUBSCRIBE MOUSE\n");
        return 1;
    }

    if(my_mouse_disable_data_reporting() != OK) {
        printf("ERROR WHILE DISABLING DATA REPORTING\n");
        return 1;
    }

    if (kbd_unsubscribe_int() != OK) {
        printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
        return 1;
    }

    if(timer_unsubscribe_int() != OK){
        printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
        return 1;
    }

    return 0;
}

void (set_game_state)(enum game_state_t state) {
    switch (state) {
        case MENU: {
            vg_set_background(backgroundMainMenu_xpm);

            play_button = create_sprite(play_white_xpm, 400, 100, 0, 0);
            date_button = create_sprite(date_white_xpm, 385, 200, 0, 0);
            quit_button = create_sprite(quit_white_xpm, 350, 300, 0, 0);
            mouse = create_sprite(mouse_xpm, 518, 384, 0, 0);

            render_sprites[0] = play_button;
            render_sprites[1] = date_button;
            render_sprites[2] = quit_button;
            render_sprites[3] = mouse;

            render_frame();
            
            break;
        }
        case GAMEPLAY: {
            vg_set_background(backgroundGameplay_xpm);

            planthero = create_sprite(planthero_xpm, 0, 0, 0, 0);
            create_zombie_hord();

            render_sprites[0] = planthero;
            
            memcpy(&render_sprites[6], zombies, sizeof(zombies));

            render_frame();

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

void (clear_game_state)(enum game_state_t state) {
    switch (state) {
        case MENU: {
            destroy_sprite(&play_button);
            render_sprites[0] = NULL;
            destroy_sprite(&date_button);
            render_sprites[1] = NULL;
            destroy_sprite(&quit_button);
            render_sprites[2] = NULL;
            destroy_sprite(&mouse);
            render_sprites[3] = NULL;
            
            break;
        }
        case GAMEPLAY: {
            W_ISPRESSED = false;
            A_ISPRESSED = false;
            S_ISPRESSED = false;
            D_ISPRESSED = false;

            destroy_sprite(&planthero);
            render_sprites[0] = NULL;

            for (int i = 0; i < 5; i++) {
                if(shots[i] != NULL) {
                    delete_shot(i);
                }
            }

            for (int i = 0; i < 10; i++) {
                if(zombies[i] != NULL) {
                    delete_zombie(i);
                }
            }

            break;
        }
        case DATE: {
            break;
        }
        
        default:
            break;
    }
}

void (render_frame)() {
    vg_clear_frame();

    vg_draw_background();
    
    for (int i = 0; i < 20; i++) {
        if (render_sprites[i] != NULL) {
            draw_sprite(render_sprites[i]);
        }
    }

    page_flip();
}

void (create_zombie_hord)() {
    int x_spawn = 908;
    int y_spawn;

    for (int i = 0; i < 2; i++) {
        y_spawn = 110;
        for (int j = 0; j < 5; j++) {
            Sprite* zombie = create_sprite(zombie_xpm, x_spawn, y_spawn, -1, 0);
            zombies[5*i + j] = zombie;

            y_spawn += 110;
        }
        x_spawn = 966;
    }
}

bool (check_hero_zombies_collisions)() {
    for (int i = 0; i < 10; i++) {
        if (zombies[i] != NULL) {
            bool collided_hero_zombie = check_sprite_collision(planthero, zombies[i]);
            if (collided_hero_zombie) {
                return true;
            }
        }
    }
    return false;
}

void (update_shots)() {
    for (int i = 0; i < 5; i++) {
        if (shots[i] != NULL) {
            update_sprite_position(shots[i]);
        }
    }
}

void (update_zombies)() {
    for (int i = 0; i < 10; i++) {
        if (zombies[i] != NULL) {
            update_sprite_position(zombies[i]);
        }
    }
}

void (manage_zombies_shot)() {
    for (int i = 0; i < 10; i++) {
        if (zombies[i] != NULL) {
            for (int j = 0; j < 5; j++) {
                if (shots[j] != NULL) {
                    bool collided_shot_zombie = check_sprite_collision(shots[j], zombies[i]);
                    if (collided_shot_zombie) {
                        delete_zombie(i);

                        delete_shot(j);

                        break;
                    }
                };
            }
        }
    }
}

void (manage_shots_at_edge)() {
    for (int i = 0; i < 5; i++) {
        if (shots[i] != NULL) {
            if (sprite_at_left_edge(shots[i]) == true) {
                delete_shot(i);
            }
        }
    }
}

void (delete_zombie)(int pos) {
    destroy_sprite(&zombies[pos]);

    render_sprites[6 + pos] = NULL;
}

void (delete_shot)(int pos) {
    destroy_sprite(&shots[pos]);
    
    render_sprites[1 + pos] = NULL;

    shots_fired--;
}

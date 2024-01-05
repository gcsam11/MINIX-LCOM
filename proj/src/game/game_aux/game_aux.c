#include "game_aux.h"

extern Sprite* render_sprites[21];

extern Sprite* mouse;
extern Sprite* play_button;
extern Sprite* quit_button;
extern Sprite* title;
extern Sprite* date_sprite[10];

extern Sprite* planthero;
extern Sprite* shots[5];
extern Sprite* zombies[10];
extern Sprite* score_sprite[4];

extern bool W_ISPRESSED, A_ISPRESSED, S_ISPRESSED, D_ISPRESSED;

extern uint8_t shots_fired;

extern uint8_t zombies_alive;

extern int hero_v;

extern int zombies_vy;

extern uint16_t score;

extern enum game_state_t game_state;

uint8_t kbd_irq_set, timer_irq_set, mouse_irq_set;

static const xpm_map_t digit_xpm_array[10] = {zero_xpm, one_xpm, two_xpm, three_xpm, four_xpm, five_xpm, six_xpm, seven_xpm, eight_xpm, nine_xpm};

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

            title = create_sprite(title_xpm, 175, 117, 0, 0);
            play_button = create_sprite(play_white_xpm, 368, 297, 0, 0);
            quit_button = create_sprite(quit_white_xpm, 306, 397, 0, 0);
            mouse = create_sprite(mouse_xpm, 518, 234, 0, 0);

            render_sprites[0] = title;
            render_sprites[1] = play_button;
            render_sprites[2] = quit_button;
            render_sprites[3] = mouse;

            Sprite* d1 = create_sprite(zero_xpm, 775, 738, 0, 0);
            Sprite* d2 = create_sprite(zero_xpm, 800, 738, 0, 0);
            Sprite* slash1 = create_sprite(bar_xpm, 825, 738, 0, 0);
            Sprite* m1 = create_sprite(zero_xpm, 850, 738, 0, 0);
            Sprite* m2 = create_sprite(zero_xpm, 875, 738, 0, 0);
            Sprite* slash2 = create_sprite(bar_xpm, 900, 738, 0, 0);
            Sprite* y1 = create_sprite(two_xpm, 925, 738, 0, 0);
            Sprite* y2 = create_sprite(zero_xpm, 950, 738, 0, 0);
            Sprite* y3 = create_sprite(zero_xpm, 975, 738, 0, 0);
            Sprite* y4 = create_sprite(zero_xpm, 1000, 738, 0, 0);

            date_sprite[0] = d1;
            date_sprite[1] = d2;
            date_sprite[2] = slash1;
            date_sprite[3] = m1;
            date_sprite[4] = m2;
            date_sprite[5] = slash2;
            date_sprite[6] = y1;
            date_sprite[7] = y2;
            date_sprite[8] = y3;
            date_sprite[9] = y4;
            render_sprites[4] = d1;
            render_sprites[5] = d2;
            render_sprites[6] = slash1;
            render_sprites[7] = m1;
            render_sprites[8] = m2;
            render_sprites[9] = slash2;
            render_sprites[10] = y1;
            render_sprites[11] = y2;
            render_sprites[12] = y3;
            render_sprites[13] = y4;

            update_date_pixmap();

            render_frame();
            
            break;
        }
        case GAMEPLAY: {
            vg_set_background(backgroundGameplay_xpm);

            planthero = create_sprite(planthero_xpm, 0, 0, 0, 0);

            render_sprites[0] = planthero;

            create_zombie_hord();

            render_sprites[16] = create_sprite(score_xpm, 734, 0, 0, 0);

            Sprite* dig1 = create_sprite(zero_xpm, 904, 0, 0, 0);
            Sprite* dig2 = create_sprite(zero_xpm, 934, 0, 0, 0);
            Sprite* dig3 = create_sprite(zero_xpm, 964, 0, 0, 0);
            Sprite* dig4 = create_sprite(zero_xpm, 994, 0, 0, 0);

            score_sprite[0] = dig1;
            score_sprite[1] = dig2;
            score_sprite[2] = dig3;
            score_sprite[3] = dig4;
            render_sprites[17] = dig1;
            render_sprites[18] = dig2;
            render_sprites[19] = dig3;
            render_sprites[20] = dig4;

            render_frame();

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
            destroy_sprite(&title);
            destroy_sprite(&play_button);
            destroy_sprite(&quit_button);
            destroy_sprite(&mouse);

            destroy_sprite(&date_sprite[0]);
            destroy_sprite(&date_sprite[1]);
            destroy_sprite(&date_sprite[2]);
            destroy_sprite(&date_sprite[3]);
            destroy_sprite(&date_sprite[4]);
            destroy_sprite(&date_sprite[5]);
            destroy_sprite(&date_sprite[6]);
            destroy_sprite(&date_sprite[7]);
            destroy_sprite(&date_sprite[8]);
            destroy_sprite(&date_sprite[9]);
            
            memset(render_sprites, 0, sizeof(render_sprites));
            
            break;
        }
        case GAMEPLAY: {
            W_ISPRESSED = false;
            A_ISPRESSED = false;
            S_ISPRESSED = false;
            D_ISPRESSED = false;

            zombies_vy = 1;
            hero_v = 5;

            score = 0;

            destroy_sprite(&planthero);

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

            destroy_sprite(&render_sprites[16]);

            destroy_sprite(&score_sprite[0]);
            destroy_sprite(&score_sprite[1]);
            destroy_sprite(&score_sprite[2]);
            destroy_sprite(&score_sprite[3]);

            memset(render_sprites, 0, sizeof(render_sprites));

            break;
        }
        
        default:
            break;
    }
}

void (render_frame)() {
    vg_clear_frame();

    vg_draw_background();
    
    for (int i = 0; i < 21; i++) {
        if (render_sprites[i] != NULL) {
            draw_sprite(render_sprites[i]);
        }
    }

    page_flip();
}

void (create_zombie_hord)() {
    int x_spawn = 882;
    int y_spawn;

    for (int i = 0; i < 2; i++) {
        y_spawn = 110;
        for (int j = 0; j < 5; j++) {
            Sprite* zombie = create_sprite(zombie_xpm, x_spawn, y_spawn, 0, zombies_vy);
            zombies[5*i + j] = zombie;

            y_spawn += 110;
        }
        x_spawn = 953;
    }

    memcpy(&render_sprites[6], zombies, sizeof(zombies));

    zombies_alive = 10;
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

bool (check_zombies_at_left_edge)() {
    for (int i = 0; i < 10; i++) {
        if (zombies[i] != NULL) {
            if (sprite_at_left_edge(zombies[i])) {
                return true;
            }
        }
    }
    return false;
}

bool (check_zombies_at_v_edge)() {
    for (int i = 0; i < 10; i++) {
        if (zombies[i] != NULL) {
            if (sprite_at_top_edge(zombies[i]) || sprite_at_bottom_edge(zombies[i])) {
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

void (advance_zombie_hord)() {
    zombies_vy *= -1;
    for (int i = 0; i < 10; i++) {
        if (zombies[i] != NULL) {
            move_sprite_left(zombies[i], 50);
            set_sprite_vy(zombies[i], zombies_vy);
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

                        score += 10;
                        update_score_pixmap();

                        break;
                    }
                };
            }
        }
    }
}

void (manage_shots_at_right_edge)() {
    for (int i = 0; i < 5; i++) {
        if (shots[i] != NULL) {
            if (sprite_at_right_edge(shots[i])) {
                delete_shot(i);
            }
        }
    }
}

void (delete_zombie)(int pos) {
    destroy_sprite(&zombies[pos]);

    render_sprites[6 + pos] = NULL;

    zombies_alive--;
}

void (delete_shot)(int pos) {
    destroy_sprite(&shots[pos]);
    
    render_sprites[1 + pos] = NULL;

    shots_fired--;
}

void (update_score_pixmap)() {
    uint16_t curr_score = score;
    if (curr_score >= 0 && curr_score <= 9999) {
        int digit;
        int cnt = 0;
        while (curr_score > 0) {
            digit = curr_score % 10;

            set_sprite_pixelmap(score_sprite[3 - cnt], digit_xpm_array[digit]);

            curr_score /= 10;
            cnt++;
        }
    }
}

void (update_date_pixmap)() {
    Date date = rtc_read_date();

    int digit;
    int cnt = 0;
    int day = date.day;
    int month = date.month;
    int year = date.year;

    while (day > 0) {
        digit = day % 10;

        set_sprite_pixelmap(date_sprite[1 - cnt], digit_xpm_array[digit]);

        day /= 10;
        cnt++;
    }

    cnt = 0;
    
    while (month > 0) {
        digit = month % 10;

        set_sprite_pixelmap(date_sprite[4 - cnt], digit_xpm_array[digit]);

        month /= 10;
        cnt++;
    }

    cnt=0;

    while (year > 0) {
        digit = year % 10;

        set_sprite_pixelmap(date_sprite[9 - cnt], digit_xpm_array[digit]);

        year /= 10;
        cnt++;
    }  
}

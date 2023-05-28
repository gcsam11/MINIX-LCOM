#include "game_aux.h"

extern Sprite* render_sprites[20];

extern Sprite* mouse;
extern Sprite* play_button;
extern Sprite* quit_button;
extern Sprite* title;

extern Sprite* planthero;
extern Sprite* shots[5];
extern Sprite* zombies[10];
extern Sprite* score_sprite[4];

static xpm_image_t font;

extern bool W_ISPRESSED, A_ISPRESSED, S_ISPRESSED, D_ISPRESSED;

extern uint8_t shots_fired;

extern uint8_t zombies_alive;

extern int zombies_vy;

extern uint16_t score;

extern enum game_state_t game_state;

uint8_t kbd_irq_set, timer_irq_set, mouse_irq_set;

static const xpm_map_t digit_xpm_array[10] = { zero_xpm,
                                               one_xpm,
                                               two_xpm,
                                               three_xpm,
                                               four_xpm,
                                               five_xpm,
                                               six_xpm,
                                               seven_xpm,
                                               eight_xpm,
                                               nine_xpm
                                                         };

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

            render_frame();
            
            break;
        }
        case GAMEPLAY: {
            vg_set_background(backgroundGameplay_xpm);

            planthero = create_sprite(planthero_xpm, 0, 0, 0, 0);

            render_sprites[0] = planthero;

            create_zombie_hord();

            Sprite* dig1 = create_sprite(zero_xpm, 984, 0, 0, 0);
            Sprite* dig2 = create_sprite(zero_xpm, 994, 0, 0, 0);
            Sprite* dig3 = create_sprite(zero_xpm, 1004, 0, 0, 0);
            Sprite* dig4 = create_sprite(zero_xpm, 1014, 0, 0, 0);

            score_sprite[0] = dig1;
            score_sprite[1] = dig2;
            score_sprite[2] = dig3;
            score_sprite[3] = dig4;
            render_sprites[16] = dig1;
            render_sprites[17] = dig2;
            render_sprites[18] = dig3;
            render_sprites[19] = dig4;

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
            render_sprites[0] = NULL;
            destroy_sprite(&play_button);
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

            zombies_vy = 1;

            score = 0;

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

            destroy_sprite(&score_sprite[0]);
            destroy_sprite(&score_sprite[1]);
            destroy_sprite(&score_sprite[2]);
            destroy_sprite(&score_sprite[3]);
            render_sprites[16] = NULL;
            render_sprites[17] = NULL;
            render_sprites[18] = NULL;
            render_sprites[19] = NULL;

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

void get_date() {

  Date date = rtc_read_date();
  char date_str[20];

  char day[3];
  char month[3];
  char year[5];

  sprintf(day, "%02d", date.day);
  sprintf(month, "%02d", date.month);
  sprintf(year, "%02d", date.year);

  strcpy(date_str, day);
  strcat(date_str, "/");
  strcat(date_str, month);
  strcat(date_str, "/");
  strcat(date_str, year);
}

void draw_string(const char *string, uint16_t x, uint16_t y, uint8_t scale) {
  for (unsigned int i = 0; i < strlen(string); i++) {
    if (string[i] != ' ' && string[i] != '/') {
      draw_character(string[i], x + (scale * 6 * i) , y, scale);
    } else {
      i++;
      draw_character(string[i], x + (scale * 6 * i) , y, scale);
    }
  }
}

void draw_character(const char character, uint16_t x, uint16_t y, uint8_t scale) {
  uint8_t *pnt = NULL;
  uint8_t font_bpp = (font.size / (font.height * font.width));
  unsigned char *init_pnt;

  init_pnt = font.bytes;
  
  if (!is_alpha(character)) {
    int num = character - '0';
    pnt = init_pnt + (6 * num) * font_bpp;
  } else {
    int num = (int) character - 65;
    if (num < 0 || num > 25) return;

    if (num >=0 && num <= 14) {
      pnt = init_pnt + (6 * (num + 1)) * font_bpp + (font.width * 7) * font_bpp;
    } else if (num >= 15 && num <=25) {
      pnt = init_pnt + (6 * (num - 15)) * font_bpp + (font.width * 14) * font_bpp;
    }
  }

  vg_draw_character(font, x, y, scale, pnt);
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

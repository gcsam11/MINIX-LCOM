// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "keyboard.h"
#include "video_gr.h"
#include "sprite.h"
#include "i8042.h"

extern uint8_t scancode;
extern uint32_t timer0_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if(vg_init(mode) == NULL) {
    printf("FAILED TO INITIALIZE THE GRAPHICS MODE\n");
    return 1;
  }
  
  sleep(delay);

  if(vg_exit()) {
    printf("FAILED TO GO BACK TO MINIX DEFAULT TEXT MODE\n");
    return 1;
  }
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  if(vg_init(mode) == NULL) {
    printf("FAILED TO INITIALIZE THE GRAPHICS MODE\n");
    return 1;
  }

  vg_draw_rectangle(x, y, width, height, color);

  int ipc_status;
  int r, k = 0;
  uint8_t irq_set;
  uint8_t bytes[2];
  message msg;

  if (kbd_subscribe_int(&irq_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }
  
  while(scancode != ESC_BREAK) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */	
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    kbc_ih();
                    if (scancode == KBC_2BYTECODE_FIRST) {
                      bytes[k++] = scancode;
                      continue;
                    }
                    bytes[k] = scancode;
                    kbd_print_scancode(!(scancode & KBC_ACTIVATE_BRKCODE), k + 1, bytes);
                    k = 0;
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
  }

  if (kbd_unsubscribe_int() != F_OK) {
    printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if(vg_exit()) {
    printf("FAILED TO GO BACK TO MINIX DEFAULT TEXT MODE\n");
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(vg_init(mode) == NULL) {
    printf("FAILED TO INITIALIZE THE GRAPHICS MODE\n");
    return 1;
  }

  vg_draw_rect_matrix(mode, no_rectangles, first, step);

  int ipc_status;
  int r, k = 0;
  uint8_t irq_set;
  uint8_t bytes[2];
  message msg;

  if (kbd_subscribe_int(&irq_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }
  
  while(scancode != ESC_BREAK) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */	
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    kbc_ih();
                    if (scancode == KBC_2BYTECODE_FIRST) {
                      bytes[k++] = scancode;
                      continue;
                    }
                    bytes[k] = scancode;
                    kbd_print_scancode(!(scancode & KBC_ACTIVATE_BRKCODE), k + 1, bytes);
                    k = 0;
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
  }

  if (kbd_unsubscribe_int() != F_OK) {
    printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if(vg_exit()) {
    printf("FAILED TO GO BACK TO MINIX DEFAULT TEXT MODE\n");
    return 1;
  }

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if(vg_init(0x105) == NULL) {
    printf("FAILED TO INITIALIZE THE GRAPHICS MODE\n");
    return 1;
  }

  xpm_image_t img;
  uint8_t* map = xpm_load(xpm, XPM_INDEXED, &img);

  vg_draw_xpm(x, y, img.width, img.height, map);

  int ipc_status;
  int r, k = 0;
  uint8_t irq_set;
  uint8_t bytes[2];
  message msg;

  if (kbd_subscribe_int(&irq_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }
  
  while(scancode != ESC_BREAK) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */	
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    kbc_ih();
                    if (scancode == KBC_2BYTECODE_FIRST) {
                      bytes[k++] = scancode;
                      continue;
                    }
                    bytes[k] = scancode;
                    kbd_print_scancode(!(scancode & KBC_ACTIVATE_BRKCODE), k + 1, bytes);
                    k = 0;
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
  }

  if (kbd_unsubscribe_int() != F_OK) {
    printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if(vg_exit()) {
    printf("FAILED TO GO BACK TO MINIX DEFAULT TEXT MODE\n");
    return 1;
  }

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  int ipc_status;
  int r, k = 0;
  uint8_t irq_timer_set, irq_kbc_set;
  uint8_t bytes[2];
  message msg;

  if(vg_init(0x105) == NULL) {
    printf("FAILED TO INITIALIZE THE GRAPHICS MODE\n");
    return 1;
  }

  Sprite* sp;
  bool is_x_mov = false;
  bool is_y_mov = false;
  int distance_left;

  if(xi == xf) {
    is_y_mov = true;

    distance_left = (yf - yi);

    if(speed > 0) {
      if(yi < yf)
        sp = create_sprite(xpm, xi, yi, 0, speed);
      else
        sp = create_sprite(xpm, xi, yi, 0, -speed);
    } else {
      if(yi < yf)
         sp = create_sprite(xpm, xi, yi, 0, 1);
      else 
         sp = create_sprite(xpm, xi, yi, 0, -1);
      }
  } else if(yi == yf) {
    is_x_mov = true;

    distance_left = (xf - xi);

    if(speed > 0) {
      if(xi < xf)
        sp = create_sprite(xpm, xi, yi, speed, 0);
      else
        sp = create_sprite(xpm, xi, yi, -speed, 0);
    } else {
      if(xi < xf)
        sp = create_sprite(xpm, xi, yi, 1, 0);
      else 
        sp = create_sprite(xpm, xi, yi, -1, 0);
      }
  } else {
      printf("THE MOVEMENT IS NOT LINEAR\n");
      return 1;
  }

  if (kbd_subscribe_int(&irq_kbc_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if(timer_subscribe_int(&irq_timer_set) != F_OK){
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }

  uint8_t frame_time = sys_hz() / fr_rate;
  uint8_t frame_counter = 0;

  vg_draw_xpm(sp->x, sp->y, sp->width, sp->height, sp->map);
  
  while(scancode != ESC_BREAK) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
              if (msg.m_notify.interrupts & irq_kbc_set) { /* subscribed interrupt */
                  kbc_ih();
                  if (scancode == KBC_2BYTECODE_FIRST) {
                    bytes[k++] = scancode;
                    continue;
                  }
                  bytes[k] = scancode;
                  kbd_print_scancode(!(scancode & KBC_ACTIVATE_BRKCODE), k + 1, bytes);
                  k = 0;
              }
              if (msg.m_notify.interrupts &irq_timer_set) { /* subscribed interrupt */
                timer_int_handler();

                if (speed > 0) {

                  if ((timer0_counter%frame_time == 0) && (distance_left > 0)) {
                    if (is_x_mov) {
                      distance_left = (xf - sp->x);
                      if (distance_left < speed) {
                        sp->xspeed = distance_left; 
                      }
                    } else if (is_y_mov) {
                        distance_left = (yf - sp->y);
                        if (distance_left < speed) {
                          sp->yspeed = distance_left; 
                        }
                    }
                    
                    update_sprite(sp);
                  }
                  
                } else {
                    if((timer0_counter%frame_time == 0) && (distance_left > 0))
                      frame_counter++;
                  
                    if((frame_counter == speed) && (distance_left > 0)) {
                      frame_counter = 0;

                      update_sprite(sp);

                      distance_left -= 1;
                    }
                }
                
              }
              break;
            default:
              break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
  }

  if (kbd_unsubscribe_int() != F_OK) {
    printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if(timer_unsubscribe_int() != F_OK){
    printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if(vg_exit()) {
    printf("FAILED TO GO BACK TO MINIX DEFAULT TEXT MODE\n");
    return 1;
  }

  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

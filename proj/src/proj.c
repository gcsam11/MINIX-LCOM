#include "proj.h"

extern uint32_t timer0_interrupt_cnt;
extern uint8_t scancode;

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


int(proj_main_loop)(int argc, char *argv[]) {
  int ipc_status;
  int r, k = 0;
  uint8_t timer_irq_set, kbd_irq_set;
  uint8_t bytes[2];
  message msg;

  uint8_t frame_time = sys_hz() / 60;

  if(timer_subscribe_int(&timer_irq_set) != F_OK){
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if (kbd_subscribe_int(&kbd_irq_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }

  vg_init(0x11B);

  Sprite* planthero = create_sprite(planthero_xpm, 0, 350, 1, 0);
  
  draw_sprite(planthero);

  //bool running = true;
  while(scancode != ESC_BREAK) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */	
                if (msg.m_notify.interrupts &kbd_irq_set) { /* subscribed interrupt */
                    kbd_ih();

                    if (scancode == KBC_2BYTECODE_FIRST) {
                        bytes[k++] = scancode;
                        continue;
                    }   
                    bytes[k] = scancode;
                    k = 0;
                }
                if (msg.m_notify.interrupts &timer_irq_set) { /* subscribed interrupt */
                    timer_int_handler();

                    if (timer0_interrupt_cnt%frame_time == 0) {
                        clear_sprite(planthero);

                        update_sprite_position(planthero);

                        draw_sprite(planthero);

                        swap_buffers();
                    }

                    if (planthero->x == (1280-planthero->width)) {
                        planthero->xspeed = 0;
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

  clear_sprite(planthero);

  destroy_sprite(&planthero);

  vg_exit();

  return 0;
}

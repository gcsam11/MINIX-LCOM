#include "proj.h"

#define FREQUENCY 60.0
#define FRAME_RATE 60.0
#define FIXED_TIMESTEP 1/FREQUENCY

extern uint8_t scancode;
extern uint8_t pckt_bt;
extern uint32_t timer0_interrupt_cnt;

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

  bool HERO_MOVED = false, W_ISPRESSED = false, A_ISPRESSED = false, S_ISPRESSED = false, D_ISPRESSED = false;

  uint8_t interrupts_per_frame = FREQUENCY / FRAME_RATE;

  printf("%d\n", interrupts_per_frame);

  if(timer_subscribe_int(&timer_irq_set) != OK){
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if (kbd_subscribe_int(&kbd_irq_set) != OK) {
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }

  vg_init(0x118);

  vg_set_background(background_xpm);

  Sprite* planthero = create_sprite(planthero_xpm, 0, 0, 0, 0);

  vg_draw_background();
  draw_sprite(planthero);
  page_flip();

  while(scancode != ESC_MAKECODE) {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
          if (msg.m_notify.interrupts &kbd_irq_set) {
              kbd_ih();

              if (scancode == FIRSTBYTE_2BYTECODE) {
                  bytes[k++] = scancode;
                  continue;
              }   
              bytes[k] = scancode;
              k = 0;

              switch (scancode) {
                    case W_MAKECODE:
                      W_ISPRESSED = true;
                      break;
                    case W_BREAKCODE:
                      W_ISPRESSED = false;
                      break;
                    case A_MAKECODE:
                      A_ISPRESSED = true;
                      break;
                    case A_BREAKCODE:
                      A_ISPRESSED = false;
                      break;
                    case S_MAKECODE:
                      S_ISPRESSED = true;
                      break;
                    case S_BREAKCODE:
                      S_ISPRESSED = false;
                      break;
                    case D_MAKECODE:
                      D_ISPRESSED = true;
                      break;
                    case D_BREAKCODE:
                      D_ISPRESSED = false;
                      break;
                    
                    default:
                      break;
              }
              
              HERO_MOVED = W_ISPRESSED || S_ISPRESSED || A_ISPRESSED || D_ISPRESSED;

              if (W_ISPRESSED) {

                set_sprite_vy(planthero, -240);

              } else if (S_ISPRESSED) {

                  set_sprite_vy(planthero, 240);

              } else {
                  set_sprite_vy(planthero, 0);
              }

              if (A_ISPRESSED) {

                set_sprite_vx(planthero, -240);

              } else if (D_ISPRESSED) {

                  set_sprite_vx(planthero, 240);

              } else {
                  set_sprite_vx(planthero, 0);
              }
          }
          if (msg.m_notify.interrupts &timer_irq_set) {
              timer_int_handler();

              if (timer0_interrupt_cnt % interrupts_per_frame == 0) {

                  if (HERO_MOVED) {
                    update_sprite_position(planthero, FIXED_TIMESTEP);
                  }
              }
              if (HERO_MOVED) {
                vg_clear_frame();

                vg_draw_background();
                draw_sprite(planthero);

                page_flip();
              }
          }
          break;

      default:
          break;
      }
    }
  }

  if (kbd_unsubscribe_int() != OK) {
  printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
  return 1;
  }

  if(timer_unsubscribe_int() != OK){
  printf("FAILED TO UNSUBSCRIBE KEYBOARD\n");
  return 1;
  }

  clear_sprite(planthero);

  destroy_sprite(&planthero);

  vg_exit();

  return 0;
}

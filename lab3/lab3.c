#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"

extern int sys_inb_cnt;
extern uint32_t timer0_counter;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
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

  kbd_print_no_sysinb(sys_inb_cnt);

  return 0;
}

int(kbd_test_poll)() {
  int k = 0;
  uint8_t bytes[2];

  do{

    if(kbd_read_data(&scancode) != 0){
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }
    if(scancode == KBC_2BYTECODE_FIRST){
      bytes[k++] = scancode;
      continue;
    }
    bytes[k] = scancode;
    kbd_print_scancode(!(scancode & KBC_ACTIVATE_BRKCODE), k+1, bytes);
    k = 0;

  } while(scancode != ESC_BREAK);

  kbd_restore_interrupts();
  kbd_print_no_sysinb(sys_inb_cnt);

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status;
  int r, k = 0;
  uint8_t irq_timer_set, irq_kbc_set;
  uint8_t bytes[2];
  message msg;

  if (kbd_subscribe_int(&irq_kbc_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }

  if(timer_subscribe_int(&irq_timer_set) != F_OK){
    printf("FAILED TO SUBSCRIBE KEYBOARD\n");
    return 1;
  }
  
  while( ((timer0_counter/sys_hz()) < n) && (scancode != ESC_BREAK) ) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
              if (msg.m_notify.interrupts &irq_timer_set) { /* subscribed interrupt */
                timer_int_handler();
              }
              if (msg.m_notify.interrupts & irq_kbc_set) { /* subscribed interrupt */
                  kbc_ih();
                  if (scancode == KBC_2BYTECODE_FIRST) {
                    bytes[k++] = scancode;
                    continue;
                  }
                  bytes[k] = scancode;
                  kbd_print_scancode(!(scancode & KBC_ACTIVATE_BRKCODE), k + 1, bytes);
                  k = 0;
                  timer0_counter = 0;
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

  kbd_print_no_sysinb(sys_inb_cnt);

  return 0;
}

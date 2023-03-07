#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern uint32_t timer0_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t status = 0;

  if (timer_get_conf(timer, &status) != F_OK) {
    printf("AN ERROR OCURRED WHEN GETTING THE TIMER'S CONFIGURATION\n");
    return 1;
  }

  if (timer_display_conf(timer, status, field) != F_OK) {
    printf("AN ERROR OCCURED WHEN DISPLAYING THE TIMER'S CONFIGURATION");
    return 1;
  }
  
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
    
  return timer_set_frequency(timer, freq);
}

int(timer_test_int)(uint8_t time) {
  int ipc_status;
  int r;
  uint8_t irq_set;
  message msg;

  if (timer_subscribe_int(&irq_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE TIMER 0 INTERRUPT\n");
    return 1;
  }
  

  while(time) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */	
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    timer_int_handler();
                    if (timer0_counter % 60 == 0) {
                      time--;
                      timer_print_elapsed_time();
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

  if (timer_unsubscribe_int() != F_OK) {
    printf("FAILED TO UNSUBSCRIBE TIMER 0 INTERRUPT\n");
    return 1;
  }

  return 0;
}

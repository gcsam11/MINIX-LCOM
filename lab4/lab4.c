// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"
#include "i8042.h"

extern uint8_t pckt_bt;
extern uint32_t timer0_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  int r, ipc_status, bytes_read_cnt = 0;
  uint8_t irq_set;
  message msg;
  struct packet pp;
  bool store_pckt_bt = false;

  if (my_mouse_enable_data_reporting() != F_OK) {
    printf("ERROR WHILE ENABLING DATA REPORTING\n");
    return 1;
  }

  if (mouse_subscribe_int(&irq_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE MOUSE\n");
    return 1;
  }
  
  while(cnt != 0) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */	
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    mouse_ih();

                    if (pckt_bt & MOUSE_PCKT_3) {
                      store_pckt_bt = true;
                    }

                    if (store_pckt_bt) {
                      pp.bytes[bytes_read_cnt++] = pckt_bt;
                    }

                    if (bytes_read_cnt == 3) {
                      mouse_parse_packet(&pp);
                      mouse_print_packet(&pp);
                      cnt--;
                      bytes_read_cnt = 0;
                      store_pckt_bt = false;
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

  if (mouse_unsubscribe_int() != F_OK) {
    printf("FAILED TO UNSUBSCRIBE MOUSE\n");
    return 1;
  }
  
  if(my_mouse_disable_data_reporting() != F_OK) {
    printf("ERROR WHILE DISABLING DATA REPORTING\n");
    return 1;
  }

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int r, ipc_status, bytes_read_cnt = 0;
  uint8_t irq_timer_set, irq_mouse_set;
  message msg;
  struct packet pp;
  bool store_pckt_bt = false;

  if (my_mouse_enable_data_reporting() != F_OK) {
    printf("ERROR WHILE ENABLING DATA REPORTING\n");
    return 1;
  }

  if (mouse_subscribe_int(&irq_mouse_set) != F_OK) {
    printf("FAILED TO SUBSCRIBE MOUSE\n");
    return 1;
  }

  if(timer_subscribe_int(&irq_timer_set)){
    printf("FAILED TO SUBSCRIBE TIMER\n");
    return 1;
  }
  
  while(timer0_counter/sys_hz() < idle_time) {
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
                if (msg.m_notify.interrupts & irq_mouse_set) { /* subscribed interrupt */
                    mouse_ih();

                    if (pckt_bt & MOUSE_PCKT_3) {
                      store_pckt_bt = true;
                    }

                    if (store_pckt_bt) {
                      pp.bytes[bytes_read_cnt++] = pckt_bt;
                    }

                    if (bytes_read_cnt == 3) {
                      mouse_parse_packet(&pp);
                      mouse_print_packet(&pp);
                      bytes_read_cnt = 0;
                      store_pckt_bt = false;
                    }

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

  if(timer_unsubscribe_int() != F_OK){
    printf("FAILED TO UNSUBSCRIBE TIMER\n");
    return 1;
  }

  if (mouse_unsubscribe_int() != F_OK) {
    printf("FAILED TO UNSUBSCRIBE MOUSE\n");
    return 1;
  }
  
  if(my_mouse_disable_data_reporting() != F_OK) {
    printf("ERROR WHILE DISABLING DATA REPORTING\n");
    return 1;
  }

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    int r, ipc_status, bytes_read_cnt = 0, writting_gesture = 1;
    uint8_t irq_set;
    message msg;
    struct packet pp;
    struct mouse_ev* event;
    struct Gesture gesture;
    gesture.state = INITIAL;
    gesture.delta_x = 0;
    gesture.delta_y = 0;
    bool store_pckt_bt = false;

    if (my_mouse_enable_data_reporting() != F_OK) {
      printf("ERROR WHILE ENABLING DATA REPORTING\n");
      return 1;
    }

    if (mouse_subscribe_int(&irq_set) != F_OK) {
      printf("FAILED TO SUBSCRIBE MOUSE\n");
      return 1;
    }
    
    while(writting_gesture) {
      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
      }

      if (is_ipc_notify(ipc_status)) { /* received notification */
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */	
                  if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                      mouse_ih();

                      if (pckt_bt & MOUSE_PCKT_3) {
                        store_pckt_bt = true;
                      }

                      if (store_pckt_bt) {
                        pp.bytes[bytes_read_cnt++] = pckt_bt;
                      }

                      if (bytes_read_cnt == 3) {
                        mouse_parse_packet(&pp);

                        event = mouse_detect_event(&pp);
                        writting_gesture = mouse_gesture_matching(&gesture, event, x_len, tolerance);

                        mouse_print_packet(&pp);
                        bytes_read_cnt = 0;
                        store_pckt_bt = false;
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

    if (mouse_unsubscribe_int() != F_OK) {
      printf("FAILED TO UNSUBSCRIBE MOUSE\n");
      return 1;
    }
    
    if(my_mouse_disable_data_reporting() != F_OK) {
      printf("ERROR WHILE DISABLING DATA REPORTING\n");
      return 1;
    }

    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

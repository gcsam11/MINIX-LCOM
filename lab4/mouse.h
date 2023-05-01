#include <lcom/lcf.h>

enum state_t {
    INITIAL,
    LEFT_SLOPE,
    VERTEX,
    RIGHT_SLOPE,
    FINISHED
};

struct Gesture {
    enum state_t state;
    int16_t delta_x, delta_y;
};


int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int(kbc_issue_cmd)(int kbc_cmd_reg, uint8_t cmd);

int(mouse_send_cmd)(uint8_t cmd);

int my_mouse_enable_data_reporting();

int my_mouse_disable_data_reporting();

int(mouse_read_data)(uint8_t *data);

void (mouse_ih)();

void (mouse_parse_packet)(struct packet* pp);

int (mouse_gesture_matching)(struct Gesture *gesture, struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

int (handle_LB_PRESSED)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance);

int (handle_LB_RELEASED)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance);

int (handle_RB_PRESSED)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance);

int (handle_RB_RELEASED)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance);

int (handle_BUTTON_EV)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance);

int (handle_MOUSE_MOV)(struct Gesture *gesture, int16_t ev_delta_x, int16_t ev_delta_y, uint8_t x_len, uint8_t tolerance);

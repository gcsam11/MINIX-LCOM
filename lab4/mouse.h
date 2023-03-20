#include <lcom/lcf.h>

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int(kbc_issue_cmd)(int kbc_cmd_reg, uint8_t cmd);

int(mouse_send_cmd)(uint8_t cmd);

int my_mouse_enable_data_reporting();

int my_mouse_disable_data_reporting();

int(mouse_read_data)(uint8_t *data);

void (mouse_ih)();

void (mouse_parse_packet)(struct packet* pp);

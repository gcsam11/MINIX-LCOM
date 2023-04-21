#include "proj.h"


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
  vg_init(0x14c);

  xpm_image_t img;
  uint8_t* map = xpm_load(Enemy, XPM_8_8_8_8, &img);

  if (map == NULL) {
    vg_exit();

    return 0;
  }
  
  vg_draw_pixel_map(0, 0, img.width, img.height, map);

  sleep(3);

  vg_clear_pixel_map(0, 0, img.width, img.height);

  sleep(3);

  vg_exit();

  return 0;
}

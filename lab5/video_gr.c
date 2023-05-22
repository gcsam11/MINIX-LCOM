#include "video_gr.h"

static uint16_t h_res;
static uint16_t v_res;
static uint8_t bitsPerPixel;
static uint8_t bytesPerPixel;
static unsigned vram_base;
static unsigned vram_size;
static uint8_t colorMode;
static uint8_t redMaskSize;
static uint8_t greenMaskSize;
static uint8_t blueMaskSize;
static uint8_t redFieldPosition;
static uint8_t greenFieldPosition;
static uint8_t blueFieldPosition;

static void *front_buffer;
struct minix_mem_range mr;

void* (vg_init)(uint16_t mode) {
  /* GET INFO */
  vbe_mode_info_t vmi_p;
  vbe_get_mode_info(mode, &vmi_p);

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bitsPerPixel = vmi_p.BitsPerPixel;
  bytesPerPixel = BYTES(bitsPerPixel);
  vram_base = vmi_p.PhysBasePtr;
  vram_size = h_res * v_res * bytesPerPixel;
  colorMode = vmi_p.DirectColorModeInfo;
  redMaskSize = vmi_p.RedMaskSize;
  redFieldPosition = vmi_p.RedFieldPosition;
  greenMaskSize = vmi_p.GreenMaskSize;
  greenFieldPosition = vmi_p.GreenFieldPosition;
  blueMaskSize = vmi_p.BlueMaskSize;
  blueFieldPosition = vmi_p.BlueFieldPosition;

  /* MEMORY MAPPING*/
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  int r;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  front_buffer = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(front_buffer == MAP_FAILED)
    panic("couldn't map video memory");

  memset(front_buffer,0,vram_size);

  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.ah = 0x4F;
  r86.al = 0X02;
  r86.bx = BIT(14) | mode;
  r86.intno = 0X10;

  if(sys_int86(&r86) != OK) {
    printf("Error in sys_int86()\n");
    return NULL;
  }

  if(r86.ah != 0X00) {
    printf("AN ERROR OCCURED WITH THE VBE FUNCTION\n");
    return NULL;
  }

  return front_buffer;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for(unsigned i = 0; i < height; i++) {
      vg_draw_hline(x, (y+i), width, color);
      if(i >= v_res) break;
    }

    return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    uint8_t *ptr = front_buffer;
    int offset = bytesPerPixel * (h_res*y + x);
    ptr += offset;

    for(unsigned i = 0 ; i < len; i++) {
      vg_draw_pixel(&ptr, color);
    }

    return 0;
}

int(vg_draw_pixel)(uint8_t** ptr, uint32_t color) {
  for (unsigned j = 0; j < bytesPerPixel; j++) {
    **ptr = color;
    color >>= 8;
    (*ptr)++;
  }
  return 0;
}

int (vg_draw_rect_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  unsigned rect_width = h_res/no_rectangles;
  unsigned rect_height = v_res/no_rectangles;
  uint32_t color_row_col;

  for (int row = 0; row < no_rectangles; row++) {
    for (int col = 0; col < no_rectangles; col++) {
      if (mode == 0x105) {
        color_row_col = (first + (row * no_rectangles + col) * step) % (1 << bitsPerPixel);
      } else {
        uint8_t red_row_col = (EXTRACT_COLOR(first, redFieldPosition, redMaskSize) + col*step) % BIT(redMaskSize);
        uint8_t green_row_col = (EXTRACT_COLOR(first, greenFieldPosition, greenMaskSize) + row*step) % BIT(greenMaskSize);
        uint8_t blue_row_col = (EXTRACT_COLOR(first, blueFieldPosition, blueMaskSize) + (col+row)*step) % BIT(blueMaskSize);

        color_row_col = (red_row_col << redFieldPosition) | (green_row_col << greenFieldPosition) | (blue_row_col << blueFieldPosition);
      }
      vg_draw_rectangle(col*rect_width, row*rect_height, rect_width, rect_height, color_row_col);
    }
  }
  
  return 0;
}

int (vg_draw_xpm)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h, uint8_t* map) {
  uint8_t *ptr = front_buffer;
  int offset = bytesPerPixel * (h_res*y + x);
  ptr += offset;

  for(int i = 0; i < xpm_image_h; i++){
    for(int j = 0; j < xpm_image_w; j++){
      uint32_t color = 0;
      for (unsigned byte = 0; byte < bytesPerPixel; byte++) {
        color += *(map + byte) << byte * 8;
      }
      vg_draw_pixel(&ptr, color);
      map += bytesPerPixel;
    }
    ptr -= bytesPerPixel*(xpm_image_w);
    ptr += bytesPerPixel*h_res;
  }

  return 0;
}

int (vg_clear_xpm)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h) {
  uint8_t *ptr = front_buffer;
  int offset = bytesPerPixel * (h_res*y + x);
  ptr += offset;

  for(int i = 0; i < xpm_image_h; i++){
    for(int j = 0; j < xpm_image_w; j++){
      vg_draw_pixel(&ptr, 0);
    }
    ptr -= bytesPerPixel*(xpm_image_w);
    ptr += bytesPerPixel*h_res;
  }
  return 0;
}

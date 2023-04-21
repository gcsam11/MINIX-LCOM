#include "video_card.h"

static uint16_t h_res;
static uint16_t v_res;
static uint8_t bitsPerPixel;
static uint8_t bytesPerPixel;

static void *video_mem;

void* (vg_init)(uint16_t mode) {
  /* GET INFO */
  vbe_mode_info_t vmi_p;
  vbe_get_mode_info(mode, &vmi_p);

  unsigned vram_base;
  unsigned vram_size;

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bitsPerPixel = vmi_p.BitsPerPixel;
  bytesPerPixel = BYTES(bitsPerPixel);
  vram_base = vmi_p.PhysBasePtr;
  vram_size = h_res * v_res * bytesPerPixel;

  /* MEMORY MAPPING*/

  struct minix_mem_range mr;

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  int r;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  memset(video_mem,0,vram_size);

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

  return video_mem;
}

int (vg_draw_pixel_map)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h, uint8_t* map) {
  uint8_t *ptr = video_mem;
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

int (vg_clear_pixel_map)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h) {
  uint8_t *ptr = video_mem;
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

int(vg_draw_pixel)(uint8_t** ptr, uint32_t color) {
  for (unsigned j = 0; j < bytesPerPixel; j++) {
    **ptr = color;
    color >>= 8;
    (*ptr)++;
  }
  return 0;
}


#include "video_card.h"

#define BYTES(n) (n+7)/8

static uint16_t h_res;
static uint16_t v_res;
static uint8_t bitsPerPixel;
static uint8_t bytesPerPixel;

unsigned vram_size;

static void *video_mem;
static void *buffer;

uint16_t get_h_res() {
  return h_res;
}

uint16_t get_v_res() {
  return v_res;
}

int my_vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  mmap_t m_map;

  lm_alloc(sizeof(vbe_mode_info_t), &m_map);

  r86.ax = 0x4F01;
  r86.cx = mode;
  r86.es = PB2BASE(m_map.phys);
  r86.di = PB2OFF(m_map.phys);
  r86.intno = 0x10;

  if(sys_int86(&r86) != OK) {
    printf("ERROR WITH sys_int86\n");
    return 1;
  }

  if(r86.ah != 0x00) {
    switch(r86.ah) { 
      case 0x01:
        printf("ERROR, FUNCTION CALL FAILED\n");
        break;
      case 0x02:
        printf("ERROR, FUNCTION IS NOT SUPPORTED IN CURRENT HW CONFIGURATION\n");
        break;
      case 0x03: 
        printf("ERROR, FUNCTION IS INVALID IN CURRENT VIDEO MODE\n");
        break;
      }
      lm_free(&m_map);
      return 1;
  }

  *vmi_p = *(vbe_mode_info_t*)m_map.virt;
  lm_free(&m_map);

  return 0;
}

void* (vg_init)(uint16_t mode) {
  /* GET INFO */
  vbe_mode_info_t vmi_p;
  my_vbe_get_mode_info(mode, &vmi_p);

  unsigned vram_base;

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bitsPerPixel = vmi_p.BitsPerPixel;
  bytesPerPixel = BYTES(bitsPerPixel);
  vram_base = vmi_p.PhysBasePtr;
  vram_size = h_res * v_res * bytesPerPixel;

  /* MAP MEMORY */
  struct minix_mem_range mr;

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  int r;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NULL;
  }

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED) {
    panic("couldn't map video memory");
    return NULL;
  }

  memset(video_mem, 0, vram_size);

  /* SET MODE */
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.ah = 0x4F;
  r86.al = 0X02;
  r86.bx = BIT(14) | mode;
  r86.intno = 0X10;

  if(sys_int86(&r86) != OK) {
    printf("ERROR WITH sys_int86\n");
    return NULL;
  }

  if(r86.ah != 0X00) {
    printf("AN ERROR OCCURED WITH THE VBE FUNCTION\n");
    return NULL;
  }

  buffer = (char*) malloc (vram_size);

  return video_mem;
}

void swap_buffers() {
  memcpy(video_mem, buffer, vram_size);
}

int (vg_draw_pixel_map)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h, uint8_t* map) {
  uint8_t *ptr = buffer;
  int offset = bytesPerPixel * (h_res*y + x);
  ptr += offset;

  for(int i = 0; i < xpm_image_h; i++){
    for(int j = 0; j < xpm_image_w; j++){
      uint32_t color = 0;

      memcpy(&color, map, bytesPerPixel);

      vg_draw_pixel(&ptr, color);

      map += bytesPerPixel;
    }
    ptr -= bytesPerPixel*(xpm_image_w);
    ptr += bytesPerPixel*h_res;
  }

  return 0;
}

int (vg_clear_pixel_map)(uint16_t x, uint16_t y, int xpm_image_w, int xpm_image_h) {
  uint8_t *ptr = buffer;
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
  if (color == CHROMA_KEY_GREEN_888) {
    color = 0;
  }

  memcpy(*ptr, &color, bytesPerPixel);

  *ptr += bytesPerPixel;
  
  return 0;
}

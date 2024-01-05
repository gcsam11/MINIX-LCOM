#include "video_card.h"

#define BYTES(n) (n+7)/8

static uint8_t *front_buffer;
static uint8_t *back_buffer1;
static uint8_t *back_buffer2;
static uint8_t *background_buffer;

static uint16_t h_res;
static uint16_t v_res;

static uint8_t bytesPerPixel;

static unsigned int vram_size;

uint16_t (get_h_res)() {
  return h_res;
}

uint16_t (get_v_res)() {
  return v_res;
}

int (my_vbe_get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p) {
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

  unsigned int front_buffer_base;
  unsigned int back_buffer1_base;
  unsigned int back_buffer2_base;

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;

  bytesPerPixel = BYTES(vmi_p.BitsPerPixel);

  front_buffer_base = vmi_p.PhysBasePtr;
  vram_size = h_res * v_res * bytesPerPixel;
  back_buffer1_base = front_buffer_base + vram_size;
  back_buffer2_base = back_buffer1_base + vram_size;

  /* MAP MEMORY */
  struct minix_mem_range mr;

  mr.mr_base = (phys_bytes)front_buffer_base;
  mr.mr_limit = mr.mr_base + (vram_size * 3);

  int r;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NULL;
  }

  front_buffer = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  back_buffer1 = vm_map_phys(SELF, (void *)((phys_bytes)back_buffer1_base), vram_size);
  back_buffer2 = vm_map_phys(SELF, (void *)((phys_bytes)back_buffer2_base), vram_size);

  if(front_buffer == MAP_FAILED || back_buffer1 == MAP_FAILED || back_buffer2 == MAP_FAILED) {
    panic("couldn't map video memory");
    return NULL;
  }

  memset(front_buffer, 0, vram_size);
  memset(back_buffer1, 0, vram_size);
  memset(back_buffer2, 0, vram_size);

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

  background_buffer = calloc(vram_size, sizeof(uint8_t));

  return front_buffer;
}

int (page_flip)() {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.ax = 0x4F07;  
  r.bx = 0x01;
  r.intno = 0x10;

  if(sys_int86(&r) != OK) { 
    printf("ERROR WITH sys_int86\n");
    return -1;
  }

  if(r.dx == 0) {
    memset(&r, 0, sizeof(r));
    r.dx = v_res;
  } else if (r.dx == v_res) {
      memset(&r, 0, sizeof(r));
      r.dx = v_res*2;
  } else {
      memset(&r, 0, sizeof(r));
      r.dx = 0;
  }

  r.ax = 0x4F07;
  r.bh = 0x00;
  r.bl = 0x80;
  r.cx = 0;
  r.intno = 0x10;

  if(sys_int86(&r) != OK) { 
    printf("ERROR WITH sys_int86\n");
    return -1;
  }

  uint8_t *tmp = front_buffer;
  front_buffer = back_buffer1;
  back_buffer1 = back_buffer2;
  back_buffer2 = tmp;

  return 0;
}

void (vg_set_background)(xpm_map_t xpm) {
  xpm_image_t img;
  background_buffer = xpm_load(xpm, XPM_8_8_8, &img);
}

void (vg_draw_background)() {
  memcpy(back_buffer1, background_buffer, vram_size);
}

void (vg_clear_frame)() {
  memset(back_buffer1, 0, vram_size);
}

int (vg_draw_pixel_map)(uint16_t x, uint16_t y, uint16_t xpm_image_w, uint16_t xpm_image_h, uint8_t* map) {
  int offset = bytesPerPixel * (h_res*y + x);
  
  uint8_t *vmem_ptr = back_buffer1;
  vmem_ptr += offset;

  for(int i = 0; i < xpm_image_h; i++) {
    for (int j = 0; j < xpm_image_w; j++) {
      uint32_t color = 0;

      memcpy(&color, map, bytesPerPixel);

      vg_draw_pixel(&vmem_ptr, &color);

      vmem_ptr += bytesPerPixel;
      map += bytesPerPixel;
    }
    vmem_ptr += bytesPerPixel * (h_res - xpm_image_w);
  }

  return 0;
}

int (vg_clear_pixel_map)(uint16_t x, uint16_t y, uint16_t xpm_image_w, uint16_t xpm_image_h) {
  int offset = bytesPerPixel * (h_res*y + x);

  uint8_t *vmem_ptr = front_buffer;
  const uint8_t *background_ptr = background_buffer;
  vmem_ptr += offset;
  background_ptr += offset;

  for (int i = 0; i < xpm_image_h; i++) {
    memcpy(vmem_ptr, background_ptr, bytesPerPixel * xpm_image_w );

    vmem_ptr += bytesPerPixel * h_res;
    background_ptr += bytesPerPixel * h_res;
  }

  return 0;
}

void (vg_draw_pixel)(uint8_t** vmem_ptr, uint32_t* color_ptr) {
  if (*color_ptr == CHROMA_KEY_GREEN_888) return;

  memcpy(*vmem_ptr, color_ptr, bytesPerPixel);
}

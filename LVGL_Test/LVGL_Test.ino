#include <Arduino_GFX_Library.h>
#include <lvgl.h>
 
 
// Define the data bus and display settings
Arduino_DataBus *bus = new Arduino_ESP32SPI(GFX_NOT_DEFINED /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, 19 /* MISO */, VSPI /* spi_num */);
 
Arduino_GFX *gfx = new Arduino_ST7796(bus, 4 /* RST */, 0 /* rotation */, true /* IPS */, 272, 480, 24, 0, 0, 0);
 
static void my_flush(lv_display_t *d, const lv_area_t *a, uint8_t *px) {
  uint32_t w = a->x2 - a->x1 + 1, h = a->y2 - a->y1 + 1;
  gfx->draw16bitRGBBitmap(a->x1, a->y1, (uint16_t*)px, w, h);
  lv_disp_flush_ready(d);
}
 
void setup() {
 
  gfx->begin(40000000);                 // 40 MHz; bei Instabilität 26–32 MHz
  gfx->fillScreen(BLACK);
 
  lv_init();
  static lv_color_t buf1[272 * 40];     // ~25 KB; Teil-Refresh
  auto disp = lv_display_create(272, 480);
  lv_display_set_flush_cb(disp, my_flush);
  lv_display_set_buffers(disp, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
 
  // Probe-UI
  auto lbl = lv_label_create(lv_screen_active());
  lv_label_set_text(lbl, "LVGL + ST7796 (9-bit SPI)");
  lv_obj_center(lbl);
}
 
void loop() { lv_timer_handler(); delay(5); }
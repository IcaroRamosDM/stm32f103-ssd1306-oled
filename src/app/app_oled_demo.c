#include "app/app_oled_demo.h"

#include "drivers/ssd1306/ssd1306.h"
#include "drivers/ssd1306/ssd1306_config.h"

#define APP_OLED_DEMO_PATTERN_DIVISOR   2U

#define APP_OLED_DEMO_PIXEL_ON          1U

#define APP_OLED_DEMO_TEXT_COLOR        1U
#define APP_OLED_DEMO_TITLE_X           0U
#define APP_OLED_DEMO_TITLE_Y           0U
#define APP_OLED_DEMO_LINE_1_X          0U
#define APP_OLED_DEMO_LINE_1_Y          16U
#define APP_OLED_DEMO_LINE_2_X          0U
#define APP_OLED_DEMO_LINE_2_Y          32U
#define APP_OLED_DEMO_LINE_3_X          0U
#define APP_OLED_DEMO_LINE_3_Y          48U

void app_oled_demo_checkerboard(void) {

  ssd1306_resume_display_from_ram();
  ssd1306_clear();

  for (uint8_t x = 0U; x < SSD1306_WIDTH; x++) {
    for (uint8_t y = 0U; y < SSD1306_HEIGHT; y++) {
      if (((x + y) % APP_OLED_DEMO_PATTERN_DIVISOR) == 0U) {
        ssd1306_draw_pixel(x, y, APP_OLED_DEMO_PIXEL_ON);
      }
    }
  }

  ssd1306_update();
}

void app_oled_demo_all_pixels_on(void) {

  ssd1306_test_all_pixels_on();
}

void app_oled_demo_text(void) {

  ssd1306_resume_display_from_ram();
  ssd1306_clear();

  ssd1306_draw_string(
    APP_OLED_DEMO_TITLE_X,
    APP_OLED_DEMO_TITLE_Y,
    "OLED DEMO",
    APP_OLED_DEMO_TEXT_COLOR
  );

  ssd1306_draw_string(
    APP_OLED_DEMO_LINE_1_X,
    APP_OLED_DEMO_LINE_1_Y,
    "STM32F103",
    APP_OLED_DEMO_TEXT_COLOR
  );

  ssd1306_draw_string(
    APP_OLED_DEMO_LINE_2_X,
    APP_OLED_DEMO_LINE_2_Y,
    "SSD1306 I2C",
    APP_OLED_DEMO_TEXT_COLOR
  );

  ssd1306_draw_string(
    APP_OLED_DEMO_LINE_3_X,
    APP_OLED_DEMO_LINE_3_Y,
    "5X7 FONT",
    APP_OLED_DEMO_TEXT_COLOR
  );

  ssd1306_update();
}       

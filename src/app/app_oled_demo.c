#include "app/app_oled_demo.h"

#include "drivers/ssd1306/ssd1306.h"
#include "drivers/ssd1306/ssd1306_config.h"
#include "drivers/ssd1306/ssd1306_fonts.h"

#include "bsp/systick.h"
#include "bsp/gpio.h"

#define APP_OLED_DEMO_PATTERN_DIVISOR   2U

#define APP_OLED_DEMO_PIXEL_ON          SSD1306_COLOR_WHITE

#define APP_OLED_DEMO_TEXT_COLOR         SSD1306_COLOR_WHITE
#define APP_OLED_DEMO_TITLE_X            0U
#define APP_OLED_DEMO_TITLE_Y            0U
#define APP_OLED_DEMO_LINE_1_X           0U
#define APP_OLED_DEMO_LINE_1_Y          16U
#define APP_OLED_DEMO_LINE_2_X           0U
#define APP_OLED_DEMO_LINE_2_Y          32U
#define APP_OLED_DEMO_LINE_3_X           0U
#define APP_OLED_DEMO_LINE_3_Y          48U

#define APP_OLED_DEMO_CHARSET_X                    0U
#define APP_OLED_DEMO_CHARSET_Y                    0U
#define APP_OLED_DEMO_CHARSET_LINE_HEIGHT          8U
#define APP_OLED_DEMO_CHARSET_CHARS_PER_ROW       21U
#define APP_OLED_DEMO_FIRST_PRINTABLE_CHAR        32U
#define APP_OLED_DEMO_LAST_PRINTABLE_CHAR        126U

#define APP_OLED_DEMO_SHAPE_COLOR            SSD1306_COLOR_WHITE

#define APP_OLED_DEMO_BORDER_X               0U
#define APP_OLED_DEMO_BORDER_Y               0U
#define APP_OLED_DEMO_BORDER_WIDTH         128U
#define APP_OLED_DEMO_BORDER_HEIGHT         64U

#define APP_OLED_DEMO_RECT_1_X               8U
#define APP_OLED_DEMO_RECT_1_Y              12U
#define APP_OLED_DEMO_RECT_1_WIDTH          32U
#define APP_OLED_DEMO_RECT_1_HEIGHT         20U

#define APP_OLED_DEMO_RECT_2_X              52U
#define APP_OLED_DEMO_RECT_2_Y              12U
#define APP_OLED_DEMO_RECT_2_WIDTH          28U
#define APP_OLED_DEMO_RECT_2_HEIGHT         20U

#define APP_OLED_DEMO_FILLED_RECT_X         92U
#define APP_OLED_DEMO_FILLED_RECT_Y         12U
#define APP_OLED_DEMO_FILLED_RECT_WIDTH     24U
#define APP_OLED_DEMO_FILLED_RECT_HEIGHT    20U

#define APP_OLED_DEMO_LINE_Y                44U
#define APP_OLED_DEMO_LINE_X                 8U
#define APP_OLED_DEMO_LINE_WIDTH           112U

/******** Draw X ********/

#define APP_OLED_DEMO_LINE_COLOR              SSD1306_COLOR_WHITE

#define APP_OLED_DEMO_LINE_START_X            0U
#define APP_OLED_DEMO_LINE_START_Y            0U
#define APP_OLED_DEMO_LINE_END_X            127U
#define APP_OLED_DEMO_LINE_END_Y             63U

#define APP_OLED_DEMO_LINE_2_START_X        127U
#define APP_OLED_DEMO_LINE_2_START_Y          0U
#define APP_OLED_DEMO_LINE_2_END_X            0U
#define APP_OLED_DEMO_LINE_2_END_Y           63U

/******* Draw X ********/

#define APP_OLED_DEMO_SCREEN_DELAY_MS       1500U

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

void app_oled_demo_charset(void) {
  uint8_t x = APP_OLED_DEMO_CHARSET_X;
  uint8_t y = APP_OLED_DEMO_CHARSET_Y;
  uint8_t chars_in_row = 0U;

  ssd1306_resume_display_from_ram();
  ssd1306_clear();

  for (char character = (char)APP_OLED_DEMO_FIRST_PRINTABLE_CHAR;
       character <= (char)APP_OLED_DEMO_LAST_PRINTABLE_CHAR;
       character++) {

    ssd1306_draw_char(x, y, character, APP_OLED_DEMO_TEXT_COLOR);

    x += SSD1306_FONT_WIDTH + SSD1306_FONT_SPACING;
    chars_in_row++;

    if (chars_in_row >= APP_OLED_DEMO_CHARSET_CHARS_PER_ROW) {

      chars_in_row = 0U;
      x = APP_OLED_DEMO_CHARSET_X;
      y += APP_OLED_DEMO_CHARSET_LINE_HEIGHT;
    }
  }

  ssd1306_update();
}

void app_oled_demo_shapes(void)
{
  ssd1306_resume_display_from_ram();
  ssd1306_clear();

  ssd1306_draw_rectangle(
    APP_OLED_DEMO_BORDER_X,
    APP_OLED_DEMO_BORDER_Y,
    APP_OLED_DEMO_BORDER_WIDTH,
    APP_OLED_DEMO_BORDER_HEIGHT,
    APP_OLED_DEMO_SHAPE_COLOR
  );

  ssd1306_draw_rectangle(
    APP_OLED_DEMO_RECT_1_X,
    APP_OLED_DEMO_RECT_1_Y,
    APP_OLED_DEMO_RECT_1_WIDTH,
    APP_OLED_DEMO_RECT_1_HEIGHT,
    APP_OLED_DEMO_SHAPE_COLOR
  );

  ssd1306_draw_rectangle(
    APP_OLED_DEMO_RECT_2_X,
    APP_OLED_DEMO_RECT_2_Y,
    APP_OLED_DEMO_RECT_2_WIDTH,
    APP_OLED_DEMO_RECT_2_HEIGHT,
    APP_OLED_DEMO_SHAPE_COLOR
  );

  ssd1306_fill_rectangle(
    APP_OLED_DEMO_FILLED_RECT_X,
    APP_OLED_DEMO_FILLED_RECT_Y,
    APP_OLED_DEMO_FILLED_RECT_WIDTH,
    APP_OLED_DEMO_FILLED_RECT_HEIGHT,
    APP_OLED_DEMO_SHAPE_COLOR
  );

  ssd1306_draw_horizontal_line(
    APP_OLED_DEMO_LINE_X,
    APP_OLED_DEMO_LINE_Y,
    APP_OLED_DEMO_LINE_WIDTH,
    APP_OLED_DEMO_SHAPE_COLOR
  );

  ssd1306_update();
}

void app_oled_demo_lines(void) {

    ssd1306_resume_display_from_ram();
    ssd1306_clear();

    ssd1306_draw_line(
            APP_OLED_DEMO_LINE_START_X,
            APP_OLED_DEMO_LINE_START_Y,
            APP_OLED_DEMO_LINE_END_X,
            APP_OLED_DEMO_LINE_END_Y,
            APP_OLED_DEMO_LINE_COLOR
    );

    ssd1306_draw_line(
            APP_OLED_DEMO_LINE_2_START_X,
            APP_OLED_DEMO_LINE_2_START_Y,
            APP_OLED_DEMO_LINE_2_END_X,
            APP_OLED_DEMO_LINE_2_END_Y,
            APP_OLED_DEMO_LINE_COLOR
    );

    ssd1306_update();
}

void app_oled_demo_run(void) {

    app_oled_demo_text();
    gpio_led_toggle();
    delay_ms(APP_OLED_DEMO_SCREEN_DELAY_MS);

    app_oled_demo_charset();
    gpio_led_toggle();
    delay_ms(APP_OLED_DEMO_SCREEN_DELAY_MS);

    app_oled_demo_shapes();
    gpio_led_toggle();
    delay_ms(APP_OLED_DEMO_SCREEN_DELAY_MS);

    app_oled_demo_lines();
    gpio_led_toggle();
    delay_ms(APP_OLED_DEMO_SCREEN_DELAY_MS);

    app_oled_demo_checkerboard();
    gpio_led_toggle();
    delay_ms(APP_OLED_DEMO_SCREEN_DELAY_MS);

    app_oled_demo_all_pixels_on();
    gpio_led_toggle();
    delay_ms(APP_OLED_DEMO_SCREEN_DELAY_MS);
}

#include "drivers/ssd1306/ssd1306.h"
#include "drivers/ssd1306/ssd1306_fonts.h"

#include "bsp/i2c.h"
#include "bsp/systick.h"
#include "drivers/ssd1306/ssd1306_commands.h"
#include "drivers/ssd1306/ssd1306_config.h"

#include <stddef.h>
#include <stdint.h>

#define SSD1306_INIT_DELAY_MS  100U

static void ssd1306_write_command(uint8_t command);
static void ssd1306_write_command_pair(uint8_t command, uint8_t parameter);
static void ssd1306_set_full_screen_address_window(void);
static uint8_t ssd1306_get_font_index(char character);
static int16_t ssd1306_abs_int16(int16_t value);

static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];

static void ssd1306_write_command(uint8_t command) {

  const uint8_t data[] = {
    SSD1306_COMMAND_CONTROL_BYTE,
    command,
  };

  i2c_write(SSD1306_I2C_ADDRESS, data, sizeof(data));
}

static void ssd1306_write_command_pair(uint8_t command, uint8_t parameter) {

  const uint8_t data[] = {
    SSD1306_COMMAND_CONTROL_BYTE,
    command,
    parameter,
  };

  i2c_write(SSD1306_I2C_ADDRESS, data, sizeof(data));
}

static void ssd1306_set_full_screen_address_window(void) {

  ssd1306_write_command_pair(
    SSD1306_CMD_SET_COLUMN_ADDR,
    SSD1306_PARAM_FIRST_COLUMN
  );

  ssd1306_write_command(SSD1306_PARAM_LAST_COLUMN);

  ssd1306_write_command_pair(
    SSD1306_CMD_SET_PAGE_ADDR,
    SSD1306_PARAM_FIRST_PAGE
  );

  ssd1306_write_command(SSD1306_PARAM_LAST_PAGE);
}

static int16_t ssd1306_abs_int16(int16_t value) {

    return (value < 0) ? -value : value;
}

void ssd1306_init(void) {

  delay_ms(SSD1306_INIT_DELAY_MS);

  ssd1306_write_command(SSD1306_CMD_DISPLAY_OFF);
  ssd1306_write_command_pair(SSD1306_CMD_SET_DISPLAY_CLOCK_DIV, SSD1306_PARAM_CLOCK_DIV_DEFAULT);
  ssd1306_write_command_pair(SSD1306_CMD_SET_MULTIPLEX_RATIO, SSD1306_PARAM_MULTIPLEX_64);
  ssd1306_write_command_pair(SSD1306_CMD_SET_DISPLAY_OFFSET, SSD1306_PARAM_DISPLAY_OFFSET_NONE);
  ssd1306_write_command(SSD1306_CMD_SET_START_LINE);
  ssd1306_write_command_pair(SSD1306_CMD_CHARGE_PUMP, SSD1306_PARAM_CHARGE_PUMP_ENABLE);
  ssd1306_write_command_pair(SSD1306_CMD_MEMORY_MODE, SSD1306_PARAM_MEMORY_MODE_HORIZONTAL);
  ssd1306_write_command(SSD1306_CMD_SEG_REMAP);
  ssd1306_write_command(SSD1306_CMD_COM_SCAN_DEC);
  ssd1306_write_command_pair(SSD1306_CMD_SET_COM_PINS, SSD1306_PARAM_COM_PINS_128X64);
  ssd1306_write_command_pair(SSD1306_CMD_SET_CONTRAST, SSD1306_PARAM_CONTRAST_DEFAULT);
  ssd1306_write_command_pair(SSD1306_CMD_SET_PRECHARGE, SSD1306_PARAM_PRECHARGE_DEFAULT);
  ssd1306_write_command_pair(SSD1306_CMD_SET_VCOM_DESELECT, SSD1306_PARAM_VCOM_DESELECT_DEFAULT);
  ssd1306_write_command(SSD1306_CMD_DISPLAY_ALL_ON_RESUME);
  ssd1306_write_command(SSD1306_CMD_NORMAL_DISPLAY);

  ssd1306_clear();
  ssd1306_update();

  ssd1306_write_command(SSD1306_CMD_DISPLAY_ON);
}

void ssd1306_clear(void) {
  for (uint16_t i = 0U; i < SSD1306_BUFFER_SIZE; i++) {
      
      ssd1306_buffer[i] = 0x00U;
  }
}

void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {

  if ((x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT)) {
    return;
  }

  const uint16_t index = x + (y / 8U) * SSD1306_WIDTH;

  if (color != 0U) {
    ssd1306_buffer[index] |= (1U << (y % 8U));
  } else {
    ssd1306_buffer[index] &= ~(1U << (y % 8U));
  }
}

static uint8_t ssd1306_get_font_index(char character) {

    if ((character < (char)SSD1306_FONT_FIRST_CHAR) ||
        (character > (char)SSD1306_FONT_LAST_CHAR)) {

        return 0U;
    }

    return (uint8_t)character - SSD1306_FONT_FIRST_CHAR;
}

void ssd1306_draw_char(uint8_t x, uint8_t y, char character, uint8_t color) {
  const uint8_t font_index = ssd1306_get_font_index(character);

  for (uint8_t column = 0U; column < SSD1306_FONT_WIDTH; column++) {
    const uint8_t column_data = ssd1306_font_5x7[font_index][column];

    for (uint8_t row = 0U; row < SSD1306_FONT_HEIGHT; row++) {
      if ((column_data & (1U << row)) != 0U) {
        ssd1306_draw_pixel(x + column, y + row, color);
      }
    }
  }
}

void ssd1306_draw_string(uint8_t x, uint8_t y, const char *text, uint8_t color) {
  uint8_t cursor_x = x;

  while (*text != '\0') {

    ssd1306_draw_char(cursor_x, y, *text, color);
    cursor_x += SSD1306_FONT_WIDTH + SSD1306_FONT_SPACING;
    text++;
  }
}

void ssd1306_update(void) {

  ssd1306_set_full_screen_address_window();

  for (uint8_t page = 0U; page < SSD1306_PAGE_COUNT; page++) {
    uint8_t data[SSD1306_WIDTH + 1U];

    data[0] = SSD1306_DATA_CONTROL_BYTE;

    for (uint8_t column = 0U; column < SSD1306_WIDTH; column++) {

      data[column + 1U] = ssd1306_buffer[
        page * SSD1306_WIDTH + column
      ];
    }

    i2c_write(SSD1306_I2C_ADDRESS, data, sizeof(data));
  }
}

void ssd1306_test_all_pixels_on(void) {

    ssd1306_write_command(SSD1306_CMD_DISPLAY_ALL_ON);
}

void ssd1306_resume_display_from_ram(void) {

    ssd1306_write_command(SSD1306_CMD_DISPLAY_ALL_ON_RESUME);
}

void ssd1306_draw_horizontal_line(uint8_t x, uint8_t y, uint8_t width, uint8_t color)
{
  for (uint8_t offset = 0U; offset < width; offset++)
  {
    ssd1306_draw_pixel(x + offset, y, color);
  }
}

void ssd1306_draw_vertical_line(uint8_t x, uint8_t y, uint8_t height, uint8_t color)
{
  for (uint8_t offset = 0U; offset < height; offset++)
  {
    ssd1306_draw_pixel(x, y + offset, color);
  }
}

void ssd1306_draw_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
  if ((width == 0U) || (height == 0U))
  {
    return;
  }

  ssd1306_draw_horizontal_line(x, y, width, color);
  ssd1306_draw_horizontal_line(x, y + height - 1U, width, color);
  ssd1306_draw_vertical_line(x, y, height, color);
  ssd1306_draw_vertical_line(x + width - 1U, y, height, color);
}

void ssd1306_fill_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
  for (uint8_t row = 0U; row < height; row++)
  {
    ssd1306_draw_horizontal_line(x, y + row, width, color);
  }
}

void ssd1306_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {

    int16_t current_x = x0;
    int16_t current_y = y0;

    const int16_t target_x = x1;
    const int16_t target_y = y1;

    const int16_t delta_x = ssd1306_abs_int16(target_x - current_x);
    const int16_t delta_y = -ssd1306_abs_int16(target_y - current_y);

    const int16_t step_x = (current_x < target_x) ? 1 : -1;
    const int16_t step_y = (current_y < target_y) ? 1 : -1;

    int16_t error = delta_x + delta_y;

    while (1) {

        ssd1306_draw_pixel((uint8_t)current_x, (uint8_t)current_y, color);

        if ((current_x == target_x) && (current_y == target_y)) {
            break;
        }

        const int16_t double_error = 2 * error;

        if (double_error >= delta_y) {
            error += delta_y;
            current_x += step_x;
        }

        if (double_error <= delta_x) {
            error += delta_x;
            current_y += step_y;
        }
    }
}

#include "drivers/ssd1306/ssd1306.h"

#include "bsp/i2c.h"
#include "bsp/systick.h"
#include "drivers/ssd1306/ssd1306_commands.h"
#include "drivers/ssd1306/ssd1306_config.h"

#include <stddef.h>
#include <stdint.h>

#define SSD1306_INIT_DELAY_MS  100U

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
    SSD1306_COMMAND_CONTROL_BYTE,
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

  ssd1306_write_command(SSD1306_CMD_DISPLAY_ON);
}

void ssd1306_clear(void) {

  uint8_t data[SSD1306_WIDTH + 1U];

  data[0] = SSD1306_DATA_CONTROL_BYTE;

  for (size_t column = 0U; column < SSD1306_WIDTH; column++) {

    data[column + 1U] = SSD1306_PARAM_EMPTY_DATA;
  }

  ssd1306_set_full_screen_address_window();

  for (size_t page = 0U; page < SSD1306_PAGE_COUNT; page++) {

    i2c_write(SSD1306_I2C_ADDRESS, data, sizeof(data));
  }
}

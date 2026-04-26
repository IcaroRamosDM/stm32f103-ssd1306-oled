#pragma once

#include <stdint.h>

#define SSD1306_FONT_WIDTH         5U
#define SSD1306_FONT_HEIGHT        7U
#define SSD1306_FONT_SPACING       1U
#define SSD1306_FONT_FIRST_CHAR    32U
#define SSD1306_FONT_LAST_CHAR     90U

extern const uint8_t ssd1306_font_5x7[][SSD1306_FONT_WIDTH];

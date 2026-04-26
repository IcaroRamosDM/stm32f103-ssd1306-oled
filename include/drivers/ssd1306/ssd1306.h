#pragma once

#include <stdint.h>

void ssd1306_init(void);
void ssd1306_clear(void);
void ssd1306_update(void);

void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color);

void ssd1306_resume_display_from_ram(void);

void ssd1306_test_all_pixels_on(void);

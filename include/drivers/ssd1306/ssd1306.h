#pragma once

#include <stdint.h>

void ssd1306_init(void);
void ssd1306_clear(void);
void ssd1306_update(void);

void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void ssd1306_draw_char(uint8_t x, uint8_t y, char character, uint8_t color);
void ssd1306_draw_string(uint8_t x, uint8_t y, const char *text, uint8_t color);

void ssd1306_resume_display_from_ram(void);

void ssd1306_test_all_pixels_on(void);

void ssd1306_draw_horizontal_line(uint8_t x, uint8_t y, uint8_t width, uint8_t color);
void ssd1306_draw_vertical_line(uint8_t x, uint8_t y, uint8_t height, uint8_t color);
void ssd1306_draw_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void ssd1306_fill_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

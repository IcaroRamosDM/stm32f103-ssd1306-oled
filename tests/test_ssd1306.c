#include "drivers/ssd1306/ssd1306.h"
#include "drivers/ssd1306/ssd1306_config.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t image[SSD1306_BUFFER_SIZE];
static unsigned pages;

static void check(bool condition, const char *expression, int line) {
    if (!condition) {
        fprintf(stderr, "test_ssd1306.c:%d: %s\n", line, expression);
        exit(EXIT_FAILURE);
    }
}

#define CHECK(condition) check((condition), #condition, __LINE__)

void delay_ms(uint32_t milliseconds) {
    (void)milliseconds;
}

void i2c_write(uint8_t address, const uint8_t *bytes, size_t length) {
    CHECK(address == SSD1306_I2C_ADDRESS);
    CHECK(length > 0U);
    if (bytes[0] == SSD1306_DATA_CONTROL_BYTE) {
        CHECK(length == SSD1306_WIDTH + 1U);
        CHECK(pages < SSD1306_PAGE_COUNT);
        memcpy(image + pages * SSD1306_WIDTH, bytes + 1U, SSD1306_WIDTH);
        pages++;
    } else {
        CHECK(bytes[0] == SSD1306_COMMAND_CONTROL_BYTE);
    }
}

static void capture(void) {
    pages = 0U;
    ssd1306_update();
    CHECK(pages == SSD1306_PAGE_COUNT);
}

static bool pixel(unsigned x, unsigned y) {
    return (image[x + (y / SSD1306_PAGE_HEIGHT) * SSD1306_WIDTH] &
            (1U << (y % SSD1306_PAGE_HEIGHT))) != 0U;
}

static void check_empty(void) {
    capture();
    for (unsigned index = 0U; index < sizeof image; index++) {
        CHECK(image[index] == 0U);
    }
}

static void test_clipping(void) {
    ssd1306_clear();
    ssd1306_draw_horizontal_line(250U, 0U, 10U, SSD1306_COLOR_WHITE);
    ssd1306_draw_vertical_line(0U, 250U, 10U, SSD1306_COLOR_WHITE);
    ssd1306_draw_char(254U, 254U, 'W', SSD1306_COLOR_WHITE);
    ssd1306_draw_string(0U, 0U, NULL, SSD1306_COLOR_WHITE);
    check_empty();

    ssd1306_fill_rectangle(125U, 62U, 255U, 255U, SSD1306_COLOR_WHITE);
    capture();
    for (unsigned y = 0U; y < SSD1306_HEIGHT; y++) {
        for (unsigned x = 0U; x < SSD1306_WIDTH; x++) {
            CHECK(pixel(x, y) == (x >= 125U && y >= 62U));
        }
    }

    ssd1306_clear();
    ssd1306_draw_rectangle(126U, 62U, 255U, 255U, SSD1306_COLOR_WHITE);
    capture();
    CHECK(pixel(126U, 62U));
    CHECK(pixel(127U, 62U));
    CHECK(pixel(126U, 63U));
    CHECK(!pixel(127U, 63U));
    CHECK(!pixel(0U, 0U));
}

static void test_long_text_and_lines(void) {
    char long_text[81];
    memset(long_text, 'B', sizeof long_text - 1U);
    memset(long_text, 'A', 22U);
    long_text[sizeof long_text - 1U] = '\0';
    ssd1306_clear();
    ssd1306_draw_string(0U, 0U, long_text, SSD1306_COLOR_WHITE);
    capture();
    uint8_t previous[SSD1306_BUFFER_SIZE];
    memcpy(previous, image, sizeof previous);
    ssd1306_clear();
    ssd1306_draw_string(0U, 0U, "AAAAAAAAAAAAAAAAAAAAAA", SSD1306_COLOR_WHITE);
    capture();
    CHECK(memcmp(previous, image, sizeof image) == 0);

    ssd1306_clear();
    ssd1306_draw_line(0U, 0U, 127U, 63U, SSD1306_COLOR_WHITE);
    capture();
    CHECK(pixel(0U, 0U));
    CHECK(pixel(127U, 63U));
    ssd1306_clear();
    check_empty();
}

int main(void) {
    test_clipping();
    test_long_text_and_lines();
    puts("PASS: framebuffer, clipping, text bounds and simulated I2C transfers");
    return EXIT_SUCCESS;
}

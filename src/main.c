#include "app/app_oled_demo.h"
#include "bsp/clock.h"
#include "bsp/gpio.h"
#include "bsp/i2c.h"
#include "bsp/systick.h"
#include "drivers/ssd1306/ssd1306.h"
#include "drivers/ssd1306/ssd1306_config.h"

#define MAIN_LOOP_DELAY_MS 500U

int main(void) {

    clock_setup();
    systick_setup();
    gpio_setup();
    i2c_setup();

    ssd1306_init();

    while (1) {

        app_oled_demo_run();
    }

    return 0;
}

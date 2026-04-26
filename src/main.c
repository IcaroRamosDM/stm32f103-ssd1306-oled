#include "bsp/clock.h"
#include "bsp/gpio.h"
#include "bsp/i2c.h"
#include "bsp/systick.h"

#define MAIN_LOOP_DELAY_MS      500U

int main(void) {
    
    clock_setup();
    systick_setup();
    gpio_setup();
    i2c_setup();

    while (1) {
        gpio_led_toggle();
        delay_ms(MAIN_LOOP_DELAY_MS);
    }

    return 0;
}

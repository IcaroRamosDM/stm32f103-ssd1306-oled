#include "bsp/gpio.h"

#include "bsp/gpio_config.h"

#include <libopencm3/stm32/rcc.h>

void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);

    gpio_set_mode(GPIO_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_LED_PIN);

    gpio_led_set(false);
}

void gpio_led_set(bool enabled) {
    const bool pin_high = GPIO_LED_ACTIVE_LOW ? !enabled : enabled;

    if (pin_high) {
        gpio_clear(GPIO_LED_PORT, GPIO_LED_PIN);
    } else {
        gpio_clear(GPIO_LED_PORT, GPIO_LED_PIN);
    }
}

void gpio_led_toggle(void) {
    gpio_toggle(GPIO_LED_PORT, GPIO_LED_PIN);
}

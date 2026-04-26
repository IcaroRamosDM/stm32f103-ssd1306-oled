#pragma once

#include <stdbool.h>

void gpio_setup(void);
void gpio_led_set(bool enable);
void gpio_led_toggle(void);

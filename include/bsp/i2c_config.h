#pragma once

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>

#define BSP_I2C_PORT I2C1
#define BSP_I2C_RCC RCC_I2C1

#define BSP_I2C_GPIO_RCC RCC_GPIOB
#define BSP_I2C_GPIO_PORT GPIOB
#define BSP_I2C_SCL_PIN GPIO6
#define BSP_I2C_SDA_PIN GPIO7

#define BSP_I2C_APB1_FREQUENCY_MHZ 36U
#define BSP_I2C_STANDARD_SPEED_HZ 100000U

#include "bsp/i2c.h"

#include "bsp/i2c_config.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>

void i2c_setup(void) {

  rcc_periph_clock_enable(BSP_I2C_GPIO_RCC);
  rcc_periph_clock_enable(BSP_I2C_RCC);

  gpio_set_mode(
    BSP_I2C_GPIO_PORT,
    GPIO_MODE_OUTPUT_50_MHZ,
    GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
    BSP_I2C_SCL_PIN | BSP_I2C_SDA_PIN
  );

  i2c_peripheral_disable(BSP_I2C_PORT);

  i2c_set_clock_frequency(BSP_I2C_PORT, BSP_I2C_APB1_FREQUENCY_MHZ);
  i2c_set_standard_mode(BSP_I2C_PORT);
  i2c_set_ccr(BSP_I2C_PORT, BSP_I2C_APB1_FREQUENCY_MHZ * 5U);
  i2c_set_trise(BSP_I2C_PORT, BSP_I2C_APB1_FREQUENCY_MHZ + 1U);

  i2c_peripheral_enable(BSP_I2C_PORT);
}

void i2c_write(uint8_t address, const uint8_t *data, size_t length) {

  i2c_transfer7(BSP_I2C_PORT, address, data, length, NULL, 0U);
}

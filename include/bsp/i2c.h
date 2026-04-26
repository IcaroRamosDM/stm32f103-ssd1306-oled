#pragma once

#include <stddef.h>
#include <stdint.h>

void i2c_setup(void);
void i2c_write(uint8_t address, const uint8_t *data, size_t length);

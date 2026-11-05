# Wiring

| SSD1306 module | STM32F103 |
|---|---|
| VCC | Compatible 3.3 V supply |
| GND | GND |
| SCL | PB6 |
| SDA | PB7 |

The default 7-bit I²C address is 0x3C. Check the actual module address and pull-up
voltage before connecting it. Configuration is in
include/drivers/ssd1306/ssd1306_config.h.

The framebuffer is 128 columns by 64 rows, grouped into eight pages of eight
vertical pixels. update sends the address window and one 128-byte payload per page.
Host fixtures inspect those transfers without accessing the I²C peripheral.

# SSD1306 Wiring Guide

This project uses a 128x64 SSD1306 OLED display with I2C communication on the STM32F103C8 Blue Pill.

---

## Connections

| SSD1306 OLED | STM32F103C8 Blue Pill |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SCL | PB6 |
| SDA | PB7 |

---

## I2C Peripheral

This project uses:

```text
I2C1
```

Default STM32F103 mapping:

SCL -> PB6
SDA -> PB7

---

## OLED I2C Address

Most SSD1306 modules use one of these addresses: 

```text
0x3C
0x3D
```

This project uses:

```text
0x3C
```

defined in:

```text
include/drivers/ssd1306/ssd1306_config.h
```

If your display does not respond, verify the module address.

---

## Power Notes

Recommended:

```text
Use 3.3V
```

Some modules accept 5V on VCC, but using 3.3V is safer for the STM32F103.

Avoid:

```text
5V logic directly on STM32 GPIO pins
```

The STM32F103 is a 3.3V device.

---

## Pull-up Resistors

Many SSD1306 I2C modules already include pull-up resistors on:

```text
SCL
SDA
```

If communication fails, verify:

- pull-up resistors exist
- wiring is short and clean
- GND is shared
- power supply is stable

Typical pull-up values:

```text
4.7kΩ
10kΩ
```

---

## Reset Pin

Some OLED modules expose a RESET pin.

This project does not use hardware reset.

Initialization is performed entirely by software commands during:

```c
ssd1306_init();
```

Because of this, the display may preserve old content after MCU reset or flash erase if OLED power remains active.

This behavior is normal.

---

## ST-Link Flashing

Programming is done using:

```text
ST-Link V2
```

Commands:

```bash
make flash
make erase
make reflash
```

---

## Recommended Validation Order

For first bring-up:

1. Verify power (3.3V + GND)
2. Verify I2C wiring (PB6/PB7)
3. Test "all pixels on"
4. Test checkerboard pattern
5. Test text rendering
6. Run rotating demo

This makes debugging much easier.

---

## Troubleshooting

### OLED remains blank

Check:

- wiring
- I2C address
- display power
- I2C pull-ups
- ssd1306_init() execution

---

### OLED keeps old image after erase

This is expected.

The OLED controller has its own internal display RAM and keeps content while powered.

Power-cycle the OLED or explicitly clear the display in firmware.

---

### LED blinks but OLED does not respond

Usually indicates:

```text
MCU is running
but I2C communication is failing
```

Focus debugging on:

```text
SCL / SDA / address / pull-ups
```

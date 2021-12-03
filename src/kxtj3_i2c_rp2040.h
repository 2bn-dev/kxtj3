#ifndef KXTJ3_1057_I2C_RP2040_H
#define KXTJ3_1057_I2C_RP2040_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/i2c.h"


#include "kxtj3_registers.h"
#include "kxtj3_i2c.h"

#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA 8
#endif

#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL 9
#endif

#ifndef I2C_SPEED
#define I2C_SPEED 400*1000
#endif

#ifndef I2C_INTERFACE
#define I2C_INTERFACE i2c0
#endif

#ifndef I2c_READ_TIMEOUT_MS
#define I2C_READ_TIMEOUT_MS 50
#endif 


#ifndef I2C_WRITE_TIMEOUT_MS
#define I2C_WRITE_TIMEOUT_MS 50
#endif


void kxtj3_i2c_init_local();
int kxtj3_i2c_read_register_blocking_uint8(uint8_t const target_addr, uint8_t const register_addr, uint8_t length);
int kxtj3_i2c_write_register_blocking_uint8(uint8_t const target_addr, uint8_t const register_addr, uint8_t length);
int kxtj3_i2c_return_register_byte(uint8_t const register_addr);
int kxtj3_i2c_set_register_bit_mode(bool mode, uint8_t register_address, uint8_t bit_position);
#endif

#ifndef KXTJ3_1057_KXTJ3_I2C_H
#define KXTJ3_1057_KXTJ3_I2C_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#define KXTJ3_I2C_DEFAULT_ADDRESS 0x0E


#define I2C_BUF_SIZE 8
uint8_t* rxbuf;
uint8_t* txbuf;

enum I2C_STATUS_CODE {
        I2C_SUCCESS                     = 0,
        I2C_INVALID_TARGET_ADDRESS      = -1,
        I2C_INVALID_REGISTER_ADDRESS    = -2,
        I2C_RXBUF_TOO_SMALL             = -3,
        I2C_WRITE_TIMEOUT               = -4,
        I2C_READ_TIMEOUT                = -5,
        I2C_ADDR_DIDNT_ACK              = -6,
        I2C_ERROR_UNKNOWN               = -7
};

typedef enum I2C_STATUS_CODE i2c_status_code_t;

uint8_t kxtj3_get_i2c_target_address();
int kxtj3_set_i2c_target_address(uint8_t const target_address);

bool kxtj3_get_bit(uint8_t const value, uint8_t const bit_number);
uint8_t kxtj3_flip_bit(uint8_t const value, uint8_t const bit_number);

#endif

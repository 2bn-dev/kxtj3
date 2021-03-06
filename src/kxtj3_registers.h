/*! \file
 * \brief Contains information on KXTJ3 Register location and bit positions and functions to validate read and write addresses against datasheet limitations
 */

#ifndef KXTJ3_1057_KXTJ3_REGISTERS_H
#define KXTJ3_1057_KXTJ3_REGISTERS_H

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define KXTJ3_EXPECTED_DCST_OFF_RESP 0x55
#define KXTJ3_EXPECTED_DCST_ON_RESP  0xAA
#define KXTJ3_EXPECTED_WHO_AM_I_RESP 0x35
#define KXTJ3_SELF_TEST_ON_MAGIC 0xCA
#define KXTJ3_SELF_TEST_OFF_MAGIC 0x00

#define KXTJ3_REG_XOUT_L 0x06
#define KXTJ3_REG_XOUT_H 0x07
#define KXTJ3_REG_YOUT_L 0x08
#define KXTJ3_REG_YOUT_H 0x09 
#define KXTJ3_REG_ZOUT_L 0x0A
#define KXTJ3_REG_ZOUT_H 0x0B
#define KXTJ3_REG_DCST_RESP 0X0C
#define KXTJ3_REG_WHO_AM_I 0x0F
#define KXTJ3_REG_INT_SOURCE1 0x16
#define KXTJ3_REG_INT_SOURCE2 0x17
#define KXTJ3_REG_STATUS_REG 0x18
#define KXTJ3_REG_INT_REL 0x1A
#define KXTJ3_REG_CTRL_REG1 0x1B
#define KXTJ3_REG_CTRL_REG2 0x1D
#define KXTJ3_REG_INT_CTRL_REG1 0x1E
#define KXTJ3_REG_INT_CTRL_REG2 0x1F
#define KXTJ3_REG_DATA_CTRL_REG 0x21
#define KXTJ3_REG_WAKEUP_COUNTER 0x29
#define KXTJ3_REG_NA_COUNTER 0x2A
#define KXTJ3_REG_SELF_TEST 0x3A
#define KXTJ3_REG_WAKEUP_THRESHOLD_H 0x6A
#define KXTJ3_REG_WAKEUP_THRESHOLD_L 0x6B

#define KXTJ3_CTRL_REG1_BIT_PC1 7
#define KXTJ3_CTRL_REG1_BIT_RES 6
#define KXTJ3_CTRL_REG1_BIT_DRDYE 5
#define KXTJ3_CTRL_REG1_BIT_GSEL1 4
#define KXTJ3_CTRL_REG1_BIT_GSEL0 3
#define KXTJ3_CTRL_REG1_BIT_EN16G 2
#define KXTJ3_CTRL_REG1_BIT_WUFE 1

#define KXTJ3_CTRL_REG2_BIT_SRST 7
#define KXTJ3_CTRL_REG2_BIT_DCST 4
#define KXTJ3_CTRL_REG2_BIT_OWUFA 2
#define KXTJ3_CTRL_REG2_BIT_OWUFB 1
#define KXTJ3_CTRL_REG2_BIT_OWUFC 0

#define KXTJ3_INT_CTRL_REG1_BIT_IEN 5
#define KXTJ3_INT_CTRL_REG1_BIT_IEA 4
#define KXTJ3_INT_CTRL_REG1_BIT_IEL 3
#define KXTJ3_INT_CTRL_REG1_BIT_STPOL 1

#define KXTJ3_INT_CTRL_REG2_BIT_ULMODE 7
#define KXTJ3_INT_CTRL_REG2_BIT_XNWUE 5 
#define KXTJ3_INT_CTRL_REG2_BIT_XPWUE 4
#define KXTJ3_INT_CTRL_REG2_BIT_YNWUE 3
#define KXTJ3_INT_CTRL_REG2_BIT_YPWUE 2
#define KXTJ3_INT_CTRL_REG2_BIT_ZNWUE 1
#define KXTJ3_INT_CTRL_REG2_BIT_ZPWUE 0

#define KXTJ3_DATA_CTRL_REG_BIT_OSAA 3
#define KXTJ3_DATA_CTRL_REG_BIT_OSAB 2
#define KXTJ3_DATA_CTRL_REG_BIT_OSAC 1
#define KXTJ3_DATA_CTRL_REG_BIT_OSAD 0


/*! Validates an I2C address against the allowed read registers in the KXTJ3 Datasheet
 *
 *
 * 
 * \param address i2c register address to validate
 * \returns bool representing if reads are allowed or not
 * \see KXJT3_VALID_READ_ADDRESSES[]
 */
bool kxtj3_valid_read_address(uint8_t address);

/*! Validates an I2C address against the allowed write registers in the KXTJ3 Datasheet
 *  
 *   
 *     
 *  \param address i2c register address to validate
 *  \returns bool representing if writes are allowed or not
 *  \see KXJT3_VALID_WRITE_ADDRESSES[]
 */
bool kxtj3_valid_write_address(uint8_t address);
#endif

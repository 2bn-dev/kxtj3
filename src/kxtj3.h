/*! \file
 * \brief Primary library headers, include this to use the library
 */

#ifndef KXTJ3_1057_KXTJ3_H
#define KXTJ3_1057_KXTJ3_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

#include "kxtj3_registers.h"
#include "kxtj3_i2c.h"

enum KXTJ3_STATUS_CODE {
	KXTJ3_INVALID_BIT_DEPTH 	= -1001,
	KXTJ3_INVALID_SET_VALUE		= -1002,
};

//Accelerometer output bit depth
typedef enum {
	KXTJ3_BIT_DEPTH_8, //!<  8 bit output depth
	KXTJ3_BIT_DEPTH_12,//!< 12 bit output depth
	KXTJ3_BIT_DEPTH_14,//!< 14 bit output depth (only available in some ranges)
} kxtj3_bit_depth_t;

/*! Acceleration Range
 * Modifies the x/y/z output response of the accelerometer
 * (see datasheet for positional / polarity details in relation to sensor position)
 *
 * ### Expected output value at 1G
 *
 * Range/Resolution | 14 bit 	| 12 bit 	| 8 bit
 * -------------|---------------|---------------|-----------------
 * +/-2G	| N/A		| +/- 1024	| +/- 64
 * +/-4G	| N/A		| +/- 512	| +/- 32
 * +/-8G	| +/-1024	| +/- 256	| +/- 16
 * +/-16G	| +/-512	| +/- 128	| +/- 8
 *
 *
 */
typedef enum {
	KXTJ3_RANGE_2G    = 0x0, //!< 0b000, Range +/- 2G
	KXTJ3_RANGE_4G    = 0x2, //!< 0b010, Range +/- 4g
	KXTJ3_RANGE_8G_1  = 0x4, //!< 0b100, Range +/- 8G configuration 1
	KXTJ3_RANGE_8G_2  = 0x6, //!< 0b110, Range +/- 8G configuration 2 - High resolution mode only, 14 bit available
	KXTJ3_RANGE_16G_1 = 0x1, //!< 0b001, Range +/- 16G configuration 1
	KXTJ3_RANGE_16G_2 = 0x3, //!< 0b011, Range +/- 16G configuration 2
	KXTJ3_RANGE_16G_3 = 0x5, //!< 0b101, Range +/- 16G configuration 3
	KXTJ3_RANGE_16G_4 = 0x7, //!< 0b111, Range +/- 16G configuration 4 - High resolution mode only, 14 bit available
} kxtj3_range_t;

/*! Output Data Rate
 *  Data Rates >= 400 Hz will force device into High Resolution Mode
 */

typedef enum {
	KXTJ3_ODR_0P781HZ    = 0x8, //!< 0b1000,  Output Data Rate = 0.781Hz, LPF Roll Off = 0.3905Hz
	KXTJ3_ODR_1P563HZ    = 0x9, //!< 0b1001,  Output Data Rate = 1.563Hz, LPF Roll Off = 0.781Hz
	KXTJ3_ODR_3P125HZ    = 0xa, //!< 0b1010,  Output Data Rate = 3.125Hz, LPF Roll Off = 1.563Hz
	KXTJ3_ODR_6P250HZ    = 0xb, //!< 0b1011,  Output Data Rate = 6.250Hz, LPF Roll Off = 3.125Hz
	KXTJ3_ODR_12P500HZ   = 0x0, //!< 0b0000,  Output Data Rate = 12.500Hz, LPF Roll Off = 6.250Hz
	KXTJ3_ODR_25P000HZ   = 0x1, //!< 0b0001,  Output Data Rate = 25.000Hz, LPF Roll Off = 12.500Hz
	KXTJ3_ODR_50P000HZ   = 0x2, //!< 0b0010,  Output Data Rate = 50.000Hz, LPF Roll Off = 25.000Hz
	KXTJ3_ODR_100P000HZ  = 0x3, //!< 0b0011,  Output Data Rate = 100.000Hz, LPF Roll Off = 50.000Hz
	KXTJ3_ODR_200P000HZ  = 0x4, //!< 0b0100,  Output Data Rate = 200.000Hz, LPF Roll Off = 100.000Hz
	KXTJ3_ODR_400P000HZ  = 0x5, //!< 0b0101,  Output Data Rate = 400.000Hz, LPF Roll Off = 200.000Hz
	KXTJ3_ODR_800P000HZ  = 0x6, //!< 0b0110,  Output Data Rate = 800.000Hz, LPF Roll Off = 400.000Hz
	KXTJ3_ODR_1600P000HZ = 0x7, //!< 0b0111,  Output Data Rate = 1600.000Hz, LPF Roll Off = 800.000Hz
} kxtj3_odr_t; 


/*! Wake up mode output data rate
 */
typedef enum {
	KXTJ3_WU_ODR_0P781HZ   = 0x0, //!< 0b000, Output Data Rate = 0.781Hz
	KXTJ3_WU_ODR_1P563HZ   = 0x1, //!< 0b001, Output Data Rate = 1.563Hz
	KXTJ3_WU_ODR_3P125HZ   = 0x2, //!< 0b010, Output Data Rate = 3.125Hz
	KXTJ3_WU_ODR_6P250HZ   = 0x3, //!< 0b011, Output Data Rate = 6.250Hz
	KXTJ3_WU_ODR_12P500HZ  = 0x4, //!< 0b100, Output Data Rate = 12.500Hz
	KXTJ3_WU_ODR_25P000HZ  = 0x5, //!< 0b101, Output Data Rate = 25.000Hz
	KXTJ3_WU_ODR_50PO00HZ  = 0x6, //!< 0b110, Output Data Rate = 50.000Hz
	KXTJ3_WU_ODR_100P000HZ = 0x7, //!< 0b111, Output Data Rate = 100.000Hz
} kxtj3_wu_odr_t;


//! Used to pass acceleration values from kxtj3_get_xyz
typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} kxtj3_xyz_t;

//! Used to pass acceleration values in Gs
typedef struct {
	double x;
	double y;
	double z;
} kxtj3_xyz_g_t;

/*! Get XYZ acceleration values
 * \param *acceleration pointer to a kxtj3_xyz_t struct used to pass the values;
 * \param bit_depth Requested bit depth (not validated, some ranges and modes only support some bit depths)
 * \returns I2C_STATUS_CODE, KXTJ3_STATUS_CODE (value < 0) or length read (value > 0)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_xyz(kxtj3_xyz_t *acceleration, kxtj3_bit_depth_t bit_depth);

/*! Transform XYZ Values to XYZ G values
 * \param acceleration pointer to a kxtj3_xyz_t struct used to pass the values in
 * \param acceleration_g pointer to a kxtj3_xyz_g_t struct used to pass the values out
 */
void kxtj3_xyz_value_to_gs(kxtj3_xyz_t *acceleration, kxtj3_xyz_g_t *acceleration_g);

/*! Convience function to get xyz in Gs instead of values
 * \param acceleration_g pointer to a kxtj3_xyz_g_t struct used to pass the values out
 * \param bit_depth Requested bit depth (not validated, some ranges and modes only support some bit depths)
 * \returns status code (value < 0) or length read (value > 0)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_xyz_g(kxtj3_xyz_g_t *acceleration_g, kxtj3_bit_depth_t bit_depth);

/*! Gets digital communication self-test register
 * \returns status code (value < 0) or byte read (value >=0))
 * \see KXTJ3_EXPECTED_DCST_OFF_RESP
 * \see KXTJ3_EXPECTED_DCST_ON_RESP
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_dcst_resp();

/*! Gets chip/supplier recognition value
 * \returns status code (value < 0) or byte read (value >=0))
 * \see KXTJ3_EXPECTED_WHO_AM_I_RESP
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_who_am_i();

/*! Gets operating or standby mode configuration
 * \returns status (value < 0) or bit value of CTRL_REG1 register bit position PC1 (value == 1 is operating, value == 0 is standby)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_operating_mode();

/*! Set operating or standby mode configuration (Note: most configuration value changes require Standby mode)
 * \param mode 0 = standby, 1 = operating
 * \returns status code (value < 0) or bit value of CTRL_REG1 register bit position PC1 (value == 1 is operating, value == 0 is standby)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_set_operating_mode(bool mode);

/*! Get resolution mode
 * \returns status code (value < 0) or bit value of CTRL_REG1 register bit position RES (value == 1 is High resolution, 12 bit or 14 bit, value == 0 = Low Power, 8 bit, ODR <= 200 Hz))
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_resolution_mode();

/*! Set resolution mode
 * \returns status code (value < 0) or bit value of CTRL_REG1 register bit position RES (value == 1 is High resolution, 12 bit or 14 bit, value == 0 = Low Power, 8 bit, ODR <= 200 Hz))
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_set_resolution_mode(bool mode);

/*! Get Data Ready reporting interrupt value
 * \returns status code (value < 0) or bit value of CTRL_REG1 register bit position DRDYE
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_data_ready_interrupt();

/*! Set data ready reporting interrupt value
 * \param mode 0 = disabled, 1 = enabled
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 * \returns status code (value < 0) or bit value of CTRL_REG1 register bit position DRDYE
 */
int kxtj3_set_data_ready_interrupt(bool mode);

/*! Get acceleration range configuration value
 * \returns status code (value < 0) or enum kxtj3_range_t value (bit value of CTRL_REG_1 GSEL1, GSEL0, and EN16G)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 * \see kxtj3_range_t
 *
 */
int kxtj3_get_acceleration_range();

/*! Set acceleration range configuration value (2G, 4G, 8G, 16G) which controls Output value range
 * \param range configuration value from kxtj3_range_t enum
 * \returns status code (value < 0) or enum kxtj3_range_t value (bit value of CTRL_REG_1 GSEL1, GSEL0, and EN16G)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 * \see kxtj3_range_t
 */
int kxtj3_set_acceleration_range(kxtj3_range_t range);

/*! Get Wake up mode (motion detection) function
 * \returns status code (value < 0) or bit value of CTRL_REG1 register bit position WUFE
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_wake_up_mode();


/*! Set Wake up mode (motion detection) function
 * \param mode 0 = disabled, 1 = enabled
 * \returns status code (value < 0) or bit value of CTRL_REG1 register bit position WUFE
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_set_wake_up_mode(bool mode);

/*! Used to initiate software reset (ram reboot)
 * \param mode 0 = no action, 1 = start RAM reboot routine
 * \returns status code (value < 0) or bit value of CTRL_REG2 register bit position SRST
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_set_srst_mode(bool mode);

/*! Get digital communication self test configuration mode
 * \returns status code (value < 0) or bit value of CTRL_REG2 register bit position DCST
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 */
int kxtj3_get_dcst_mode();

/*! Set digital communication self test configuration mode
 * \param mode Toggles DCST_RESP register between 0x55 (mode = 0) and 0xAA (mode = 1) for testing purposes
 * \returns status code (value < 0) or bit value of CTRL_REG2 register bit position DCST
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 * \see kxtj3_functionality_test()
 */
int kxtj3_set_dcst_mode(bool mode);

/*! Get Wake up mode output data rate
 * \returns status code (value < 0) or bit value of CTRL_REG_2 register bit positions OWUFA, OWUFB, and OWUFC (kxtj3_wu_odr_t enum)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 * \see kxtj3_wu_odr_t
 */
int kxtj3_get_wake_up_output_data_rate();

/*! Set wake up mode output data rate
 * \param odr Wake Up mode Output data rate configuration value from kxtj3_wu_odr_t enum
 * \returns status code (value < 0) or bit value of CTRL_REG_2 register bit positions OWUFA, OWUFB, and OWUFC (kxtj3_wu_odr_t enum)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 * \see kxtj3_wu_odr_t
 */
int kxtj3_set_wake_up_output_data_rate(kxtj3_wu_odr_t odr);


int kxtj3_get_physical_interrupt_enabled();
int kxtj3_set_physical_interrupt_enabled(bool mode);
int kxtj3_get_physical_interrupt_polarity();
int kxtj3_set_physical_interrupt_polarity(bool mode);
int kxtj3_get_physical_interrupt_pulse_mode();
int kxtj3_set_physical_interrupt_pulse_mode(bool mode);
int kxtj3_get_self_test_polarity();
int kxtj3_set_self_test_polarity(bool mode);
int kxtj3_get_motion_interrupt_unlatched_mode();
int kxtj3_set_motion_interrupt_unlatched_mode(bool mode);
int kxtj3_get_motion_interrupt_x_negative();
int kxtj3_set_motion_interrupt_x_negative(bool mode);
int kxtj3_get_motion_interrupt_x_positive();
int kxtj3_set_motion_interrupt_x_positive(bool mode);
int kxtj3_get_motion_interrupt_y_negative();
int kxtj3_set_motion_interrupt_y_negative(bool mode);
int kxtj3_get_motion_interrupt_y_positive();
int kxtj3_set_motion_interrupt_y_positive(bool mode);
int kxtj3_get_motion_interrupt_z_negative();
int kxtj3_set_motion_interrupt_z_negative(bool mode);
int kxtj3_get_motion_interrupt_z_positive();
int kxtj3_set_motion_interrupt_z_positive(bool mode);

/*! Get Output data rate 
 * \returns status code (value < 0) or bit value of DATA_CTRL_REG register positions OSAA, OSAB, OSAC, OSAD (kxtj3_odr_t enum)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 * \see kxtj3_odr_t
 */
int kxtj3_get_output_data_rate();

/*! Set Output data rate 
 * \param odr Output data rate configuration value from kxtj3_odr_t enum
 * \returns status code (value < 0) or bit value of DATA_CTRL_REG register positions OSAA, OSAB, OSAC, OSAD (kxtj3_odr_t enum)
 * \see I2C_STATUS_CODE
 * \see KXTJ3_STATUS_CODE
 * \see kxtj3_odr_t
 */
int kxtj3_set_output_data_rate(kxtj3_odr_t odr);
int kxtj3_get_wake_up_counter();
int kxtj3_set_wake_up_counter(uint8_t value);
int kxtj3_get_non_activity_counter();
int kxtj3_set_non_activity_counter(uint8_t value);

int kxtj3_get_self_test_mode();
int kxtj3_set_self_test_mode(bool mode);

uint16_t kxtj3_get_wake_up_threshold();
int kxtj3_set_wake_up_threshold(uint16_t value);

/*! Startup configuration taken per TN-017 startup procedure doc
 * \returns status code (value < 0) or length of write (value > 0)
 */
int kxtj3_send_startup_command();

/*! Convenience function that does basic functionality test checking DCST and WHO_AM_I registers
 * \returns bool pass/fail
*/
bool kxtj3_functionality_test();
#endif

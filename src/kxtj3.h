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
	KXTJ3_BIT_DEPTH_8,
	KXTJ3_BIT_DEPTH_12,
	KXTJ3_BIT_DEPTH_14,
} kxtj3_bit_depth_t;

//Acceleration Range
typedef enum {
	KXTJ3_RANGE_2G    = 0x0, //0b000,
	KXTJ3_RANGE_4G    = 0x2, //0b010,
	KXTJ3_RANGE_8G_1  = 0x4, //0b100,
	KXTJ3_RANGE_8G_2  = 0x6, //0b110, // High resolution mode only, 14 bit available
	KXTJ3_RANGE_16G_1 = 0x1, //0b001,
	KXTJ3_RANGE_16G_2 = 0x3, //0b011,
	KXTJ3_RANGE_16G_3 = 0x5, //0b101,
	KXTJ3_RANGE_16G_4 = 0x7, //0b111, // High resolution mode only, 14 bit available
} kxtj3_range_t;

//Output Data Rate
//Data Rates >= 400 Hz will force device into High Resolution Mode
typedef enum {
	KXTJ3_ODR_0P781HZ    = 0x8, //0b1000, // Output Data Rate = 0.781Hz, LPF Roll Off = 0.3905Hz
	KXTJ3_ODR_1P563HZ    = 0x9, //0b1001, // Output Data Rate = 1.563Hz, LPF Roll Off = 0.781Hz
	KXTJ3_ODR_3P125HZ    = 0xa, //0b1010, // Output Data Rate = 3.125Hz, LPF Roll Off = 1.563Hz
	KXTJ3_ODR_6P250HZ    = 0xb, //0b1011, // Output Data Rate = 6.250Hz, LPF Roll Off = 3.125Hz
	KXTJ3_ODR_12P500HZ   = 0x0, //0b0000, // Output Data Rate = 12.500Hz, LPF Roll Off = 6.250Hz
	KXTJ3_ODR_25P000HZ   = 0x1, //0b0001, // Output Data Rate = 25.000Hz, LPF Roll Off = 12.500Hz
	KXTJ3_ODR_50P000HZ   = 0x2, //0b0010, // Output Data Rate = 50.000Hz, LPF Roll Off = 25.000Hz
	KXTJ3_ODR_100P000HZ  = 0x3, //0b0011, // Output Data Rate = 100.000Hz, LPF Roll Off = 50.000Hz
	KXTJ3_ODR_200P000HZ  = 0x4, //0b0100, // Output Data Rate = 200.000Hz, LPF Roll Off = 100.000Hz
	KXTJ3_ODR_400P000HZ  = 0x5, //0b0101, // Output Data Rate = 400.000Hz, LPF Roll Off = 200.000Hz
	KXTJ3_ODR_800P000HZ  = 0x6, //0b0110, // Output Data Rate = 800.000Hz, LPF Roll Off = 400.000Hz
	KXTJ3_ODR_1600P000HZ = 0x7, //0b0111, // Output Data Rate = 1600.000Hz, LPF Roll Off = 800.000Hz
} kxtj3_odr_t;


//Wake up Output Data Rate
typedef enum {
	KXTJ3_WU_ODR_0P781HZ   = 0x0, // 0b000,
	KXTJ3_WU_ODR_1P563HZ   = 0x1, // 0b001,
	KXTJ3_WU_ODR_3P125HZ   = 0x2, // 0b010,
	KXTJ3_WU_ODR_6P250HZ   = 0x3, // 0b011,
	KXTJ3_WU_ODR_12P500HZ  = 0x4, // 0b100,
	KXTJ3_WU_ODR_25P000HZ  = 0x5, // 0b101,
	KXTJ3_WU_ODR_50PO00HZ  = 0x6, // 0b110,
	KXTJ3_WU_ODR_100P000HZ = 0x7, // 0b111,
} kxtj3_wu_odr_t;


typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} kxtj3_xyz_t;

typedef struct {
	double x;
	double y;
	double z;
} kxtj3_xyz_g_t;


int kxtj3_get_xyz(kxtj3_xyz_t *acceleration, kxtj3_bit_depth_t bit_depth);
int kxtj3_get_dcst_resp();
int kxtj3_get_who_am_i();
int kxtj3_get_operating_mode();
int kxtj3_set_operating_mode(bool mode);
int kxtj3_get_resolution_mode();
int kxtj3_set_resolution_mode(bool mode);
int kxtj3_get_data_ready_interrupt();
int kxtj3_set_data_ready_interrupt(bool mode);
int kxtj3_get_acceleration_range();
int kxtj3_set_acceleration_range(kxtj3_range_t range);
int kxtj3_get_wake_up_mode();
int kxtj3_set_wake_up_mode(bool mode);
int kxtj3_set_srst_mode(bool mode);
int kxtj3_get_dcst_mode();
int kxtj3_set_dcst_mode(bool mode);
int kxtj3_get_wake_up_output_data_rate();
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
int kxtj3_get_output_data_rate();
int kxtj3_set_output_data_rate(kxtj3_odr_t odr);
int kxtj3_get_wake_up_counter();
int kxtj3_set_wake_up_counter(uint8_t value);
int kxtj3_get_non_activity_counter();
int kxtj3_set_non_activity_counter(uint8_t value);
int kxtj3_get_self_test_mode();
int kxtj3_set_self_test_mode(bool mode);
uint16_t kxtj3_get_wake_up_threshold();
int kxtj3_set_wake_up_threshold(uint16_t value);
int kxtj3_send_startup_command();
bool kxtj3_functionality_test();
#endif

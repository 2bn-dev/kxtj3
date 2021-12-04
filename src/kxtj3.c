/*! \file
 * \brief Code for primary kxtj3 library
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "kxtj3.h"
#include "kxtj3_debug.h"
#include "kxtj3_i2c_rp2040.h"


// Register: XOUT_L, XOUT_H, YOUT_L, YOUT_H, ZOUT_L, ZOUT_H

int kxtj3_get_xyz(kxtj3_xyz_t *acceleration, kxtj3_bit_depth_t bit_depth){
	uint8_t length = 6;
	
	acceleration->x = 0;
	acceleration->y = 0;
	acceleration->z = 0;

        uint8_t ret = kxtj3_i2c_read_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_XOUT_L,  length); // gets all 6 registers
        if (ret < 1)
                return ret;
	
	uint8_t x_l = rxbuf[0];
	uint8_t x_h = rxbuf[1];
	uint8_t y_l = rxbuf[2];
	uint8_t y_h = rxbuf[3];
	uint8_t z_l = rxbuf[4];
	uint8_t z_h = rxbuf[5];

	
	_DBG("x_l: 0x%02x x_h: 0x%02x y_l: 0x%02x y_h: 0x%02x z_l: 0x%02x z_h: 0x%02x", x_l, x_h, y_l, y_h, z_l, z_h);

	if(bit_depth == KXTJ3_BIT_DEPTH_8){
		acceleration->x = x_h;
		acceleration->y = y_h;
		acceleration->z = z_h;
	}else if(bit_depth == KXTJ3_BIT_DEPTH_12){
		uint16_t x = (x_l>>4 | ((x_h) << 4));
		uint16_t y = (y_l>>4 | ((y_h) << 4));
		uint16_t z = (z_l>>4 | ((z_h) << 4));

		//If MSB == 1 value is negative, two's complement means 0's should be 1's for unimplemented bits
		if((x & (1 << 11)) != 0 ) x |= 0xf000;
		if((y & (1 << 11)) != 0 ) y |= 0xf000; // 0xf000 = 1111 for first 4 bits
		if((z & (1 << 11)) != 0 ) z |= 0xf000;

		memcpy(&acceleration->x, &x, sizeof(int16_t));
		memcpy(&acceleration->y, &y, sizeof(int16_t));
		memcpy(&acceleration->z, &z, sizeof(int16_t));


	}else if(bit_depth == KXTJ3_BIT_DEPTH_14){
		uint16_t x = (x_l>>2 | ((x_h) << 6));
		uint16_t y = (y_l>>2 | ((y_h) << 6));
		uint16_t z = (z_l>>2 | ((z_h) << 6));

		if((x & (1 << 13)) != 0 ) x |= 0xc000;
		if((y & (1 << 13)) != 0 ) y |= 0xc000; // 0xc000 = 11 first 2 bits
		if((z & (1 << 13)) != 0 ) z |= 0xc000;

		memcpy(&acceleration->x, &x, sizeof(int16_t));
		memcpy(&acceleration->y, &y, sizeof(int16_t));
		memcpy(&acceleration->z, &z, sizeof(int16_t));
	}else{
		return KXTJ3_INVALID_BIT_DEPTH;
	}


        return ret;
}



// Register: DCST_RESP

int kxtj3_get_dcst_resp(){
        return kxtj3_i2c_return_register_byte(KXTJ3_REG_DCST_RESP);
}

// Register: WHO_AM_I

int kxtj3_get_who_am_i(){
        return kxtj3_i2c_return_register_byte(KXTJ3_REG_WHO_AM_I);
}





// Register: CTRL_REG_1
int kxtj3_get_operating_mode(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1), KXTJ3_CTRL_REG1_BIT_PC1);
}

int kxtj3_set_operating_mode(bool mode){
	//true == operating
	//false == standby
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_CTRL_REG1, KXTJ3_CTRL_REG1_BIT_PC1);
}

int kxtj3_get_resolution_mode(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1), KXTJ3_CTRL_REG1_BIT_RES);
}

int kxtj3_set_resolution_mode(bool mode){
	//true == high resolution (12 bit or 14 bit, Bandwidth = ODR/2 hz)
	//false == low resolution/power (8 Bit) - ODR <= 200 Hz only. Bandwidth = 800 Hz
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_CTRL_REG1, KXTJ3_CTRL_REG1_BIT_RES);
}

int kxtj3_get_data_ready_interrupt(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1), KXTJ3_CTRL_REG1_BIT_DRDYE);
}

int kxtj3_set_data_ready_interrupt(bool mode){
	//true = availability of new acceleration data is reflected as an interrupt
	//false = availability of new acceleration data is not reflected as an interrupt
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_CTRL_REG1, KXTJ3_CTRL_REG1_BIT_DRDYE);
}

int kxtj3_get_acceleration_range(){
        int ret = kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1);
	if(ret < 0) return ret;

	int gsel1 = kxtj3_get_bit(ret, KXTJ3_CTRL_REG1_BIT_GSEL1);
	int gsel0 = kxtj3_get_bit(ret, KXTJ3_CTRL_REG1_BIT_GSEL0);
	int en16g = kxtj3_get_bit(ret, KXTJ3_CTRL_REG1_BIT_EN16G);

        return (kxtj3_range_t)((gsel1<<2) | (gsel0<<1) |en16g);
}

int kxtj3_set_acceleration_range(kxtj3_range_t range){
	int value = kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1);
	uint8_t length = 1;
	if(value < 0) 
		return value;

	value |= (range << KXTJ3_CTRL_REG1_BIT_EN16G);
	txbuf[0] = value;

	int ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_CTRL_REG1, length);
        if(ret < 1)
		_DBG("WRITE ERROR %d", ret);

	return ret;
}

int kxtj3_get_wake_up_mode(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1), KXTJ3_CTRL_REG1_BIT_WUFE);
}

int kxtj3_set_wake_up_mode(bool mode){
	//0 == wake_up disabled
	//1 == wake_up enabled
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_CTRL_REG1, KXTJ3_CTRL_REG1_BIT_WUFE);
}



// Register: CTRL_REG_2

/*
Per docs SRST = 1 results in instant reboot, so no point in having this func
int get_srst_mode(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG2), KXTJ3_CTRL_REG2_BIT_SRST);
}
*/

int kxtj3_set_srst_mode(bool mode){
        //0 == nothing
        //1 == start ram reboot routine
        return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_CTRL_REG2, KXTJ3_CTRL_REG2_BIT_SRST);
}


int kxtj3_get_dcst_mode(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG2), KXTJ3_CTRL_REG2_BIT_DCST);
}

int kxtj3_set_dcst_mode(bool mode){
	//0 == DCST_RESP = 0x55
	//1 == DCST_RESP = 0xAA until read, then flips bit
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_CTRL_REG2, KXTJ3_CTRL_REG2_BIT_DCST);
}

int kxtj3_get_wake_up_output_data_rate(){
        int ret = kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG2);
        if(ret < 0) return ret;

        int owufa = kxtj3_get_bit(ret, KXTJ3_CTRL_REG2_BIT_OWUFA);
        int owufb = kxtj3_get_bit(ret, KXTJ3_CTRL_REG2_BIT_OWUFB);
        int owufc = kxtj3_get_bit(ret, KXTJ3_CTRL_REG2_BIT_OWUFC);

        return (kxtj3_wu_odr_t)((owufa<<2) | (owufb<<1) |owufc);
}

int kxtj3_set_wake_up_output_data_rate(kxtj3_wu_odr_t odr){
        int value = kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG2);
	uint8_t length = 1;
        if(value < 0) return value;

        value |= (odr);
        txbuf[0] = value;

        int ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_CTRL_REG2, length);
        if(ret < 1)
                _DBG("WRITE ERROR %d", ret);


        return ret;
}



// Register: INT_CTRL_REG1

int kxtj3_get_physical_interrupt_enabled(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1), KXTJ3_INT_CTRL_REG1_BIT_IEN);
}

int kxtj3_set_physical_interrupt_enabled(bool mode){
	//0 == INT disabled
	//1 == INT enabled
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG1, KXTJ3_INT_CTRL_REG1_BIT_IEN);
}

int kxtj3_get_physical_interrupt_polarity(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1), KXTJ3_INT_CTRL_REG1_BIT_IEA);
}
int kxtj3_set_physical_interrupt_polarity(bool mode){
	//0 == INT active LOW
	//1 == INT active HIGH
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG1, KXTJ3_INT_CTRL_REG1_BIT_IEA);
}

int kxtj3_get_physical_interrupt_pulse_mode(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1), KXTJ3_INT_CTRL_REG1_BIT_IEL);
}

int kxtj3_set_physical_interrupt_pulse_mode(bool mode){
	//0 == LATCH until cleared by reading INT_REL
	//1 == Transmit pulse of 30-50 ms
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG1, KXTJ3_INT_CTRL_REG1_BIT_IEL);
}

int kxtj3_get_self_test_polarity(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_CTRL_REG1), KXTJ3_INT_CTRL_REG1_BIT_STPOL);
}

int kxtj3_set_self_test_polarity(bool mode){
	//0 = negative polarity (unsupported?)
	//1 = positive polarity (supported?)
	
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG1, KXTJ3_INT_CTRL_REG1_BIT_STPOL);
}

// Register: INT_CTRL_REG2
	//See datasheet table 15 for details on all

int kxtj3_get_motion_interrupt_unlatched_mode(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_INT_CTRL_REG2), KXTJ3_INT_CTRL_REG2_BIT_ULMODE);
}

int kxtj3_set_motion_interrupt_unlatched_mode(bool mode){
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG2, KXTJ3_INT_CTRL_REG2_BIT_ULMODE);
}

int kxtj3_get_motion_interrupt_x_negative(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_INT_CTRL_REG2), KXTJ3_INT_CTRL_REG2_BIT_XNWUE);
}

int kxtj3_set_motion_interrupt_x_negative(bool mode){
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG2, KXTJ3_INT_CTRL_REG2_BIT_XNWUE);
}

int kxtj3_get_motion_interrupt_x_positive(){
	return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_INT_CTRL_REG2), KXTJ3_INT_CTRL_REG2_BIT_XPWUE);
}

int kxtj3_set_motion_interrupt_x_positive(bool mode){
	return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG2, KXTJ3_INT_CTRL_REG2_BIT_XPWUE);
}

int kxtj3_get_motion_interrupt_y_negative(){
        return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_INT_CTRL_REG2), KXTJ3_INT_CTRL_REG2_BIT_YNWUE);
}

int kxtj3_set_motion_interrupt_y_negative(bool mode){
        return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG2, KXTJ3_INT_CTRL_REG2_BIT_YNWUE);
}

int kxtj3_get_motion_interrupt_y_positive(){
        return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_INT_CTRL_REG2), KXTJ3_INT_CTRL_REG2_BIT_YPWUE);
}

int kxtj3_set_motion_interrupt_y_positive(bool mode){
        return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG2, KXTJ3_INT_CTRL_REG2_BIT_YPWUE);
}

int kxtj3_get_motion_interrupt_z_negative(){
        return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_INT_CTRL_REG2), KXTJ3_INT_CTRL_REG2_BIT_ZNWUE);
}

int kxtj3_set_motion_interrupt_z_negative(bool mode){
        return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG2, KXTJ3_INT_CTRL_REG2_BIT_ZNWUE);
}

int kxtj3_get_motion_interrupt_z_positive(){
        return kxtj3_get_bit(kxtj3_i2c_return_register_byte(KXTJ3_REG_INT_CTRL_REG2), KXTJ3_INT_CTRL_REG2_BIT_ZPWUE);
}

int kxtj3_set_motion_interrupt_z_positive(bool mode){
        return kxtj3_i2c_set_register_bit_mode(mode, KXTJ3_REG_INT_CTRL_REG2, KXTJ3_INT_CTRL_REG2_BIT_ZPWUE);
}


// Register: DATA_CTRL_REG

int kxtj3_get_output_data_rate(){
        int ret = kxtj3_i2c_return_register_byte(KXTJ3_REG_DATA_CTRL_REG);
        if(ret < 0) return ret;

        int osaa = kxtj3_get_bit(ret, KXTJ3_DATA_CTRL_REG_BIT_OSAA);
        int osab = kxtj3_get_bit(ret, KXTJ3_DATA_CTRL_REG_BIT_OSAB);
        int osac = kxtj3_get_bit(ret, KXTJ3_DATA_CTRL_REG_BIT_OSAC);
	int osad = kxtj3_get_bit(ret, KXTJ3_DATA_CTRL_REG_BIT_OSAD);

        return (kxtj3_odr_t)((osaa<<3) | (osab<<2) | (osac<<1) |osad);
}

int kxtj3_set_output_data_rate(kxtj3_odr_t odr){
        int value = kxtj3_i2c_return_register_byte(KXTJ3_REG_DATA_CTRL_REG);
	uint8_t length = 1;
        if(value < 0) return value;

        value |= (odr);
        txbuf[0] = value;

        int ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_DATA_CTRL_REG, length);
        if(ret < 1)
                _DBG("WRITE ERROR %d", ret);


        return ret;
}


// Register: WAKEUP_COUNTER

int kxtj3_get_wake_up_counter(){
	return kxtj3_i2c_return_register_byte(KXTJ3_REG_WAKEUP_COUNTER);
}

int kxtj3_set_wake_up_counter(uint8_t value){
	if(value == 0) // Per datasheet 0 is invalid
		return KXTJ3_INVALID_SET_VALUE;
	uint8_t length = 1;

	txbuf[0] = value;
	int ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_WAKEUP_COUNTER, length);
	if(ret < 1)
		_DBG("WRITE ERROR %d", ret);

	return ret;
}

// Register: NA_COUNTER

int kxtj3_get_non_activity_counter(){
        return kxtj3_i2c_return_register_byte(KXTJ3_REG_NA_COUNTER);
}

int kxtj3_set_non_activity_counter(uint8_t value){
        if(value == 0) // Per datasheet 0 is invalid
                return KXTJ3_INVALID_SET_VALUE;
	uint8_t length = 1;

        txbuf[0] = value;
        int ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_NA_COUNTER, length);
        if(ret < 1)
                _DBG("WRITE ERROR %d", ret);
	
        return ret;
}

// Register: SELF_TEST

int kxtj3_get_self_test_mode(){
	return (kxtj3_i2c_return_register_byte(KXTJ3_REG_SELF_TEST) == KXTJ3_SELF_TEST_ON_MAGIC);
}

int kxtj3_set_self_test_mode(bool mode){
	//0 = off (value 0x00)
	//1 = on (value 0xca)

	uint8_t length = 1;
	if(mode)
	        txbuf[0] = KXTJ3_SELF_TEST_ON_MAGIC;
	else
		txbuf[0] = KXTJ3_SELF_TEST_OFF_MAGIC;
	

	int ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_NA_COUNTER, length);
        if(ret < 1)
                _DBG("WRITE ERROR %d", ret);

        return ret;
}

uint16_t kxtj3_get_wake_up_threshold(){
	uint16_t threshold_high = kxtj3_i2c_return_register_byte(KXTJ3_REG_WAKEUP_THRESHOLD_H);
	if(threshold_high < 0)
		return threshold_high;
	uint16_t threshold_low = kxtj3_i2c_return_register_byte(KXTJ3_REG_WAKEUP_THRESHOLD_L);
	if(threshold_low < 0)
		return threshold_low;

	return (uint16_t) ((threshold_high << 4) | (threshold_low));
}

int kxtj3_set_wake_up_threshold(uint16_t value){
	uint8_t length = 2;

	txbuf[0] = ((value & 0xff0) >> 4);
	txbuf[1] = ((value & 0x00f) << 4);

	int ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_NA_COUNTER, length);
	if(ret < 1)
                _DBG("WRITE ERROR %d", ret);

	return ret;
}

int kxtj3_send_startup_command(){
	//per 
	//https://kionixfs.azureedge.net/en/document/TN017-Power-On-Procedure.pdf 0x00 should written to 0x7f during startup for $reasons?
	
	uint8_t length = 0;
	txbuf[0] = 0x00;
	int ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), KXTJ3_REG_NA_COUNTER, length);
        if(ret < 1)
		_DBG("WRITE ERROR %d", ret);
        return ret;
}


bool kxtj3_functionality_test(){
	kxtj3_set_dcst_mode(false);
	if(kxtj3_get_dcst_resp() != KXTJ3_EXPECTED_DCST_OFF_RESP){
		return false;
	}
	
	kxtj3_set_dcst_mode(true);

	if(kxtj3_get_dcst_resp() != KXTJ3_EXPECTED_DCST_ON_RESP){
		return false;
	}

	if(kxtj3_get_who_am_i() != KXTJ3_EXPECTED_WHO_AM_I_RESP){
		return false;
	}

	return true;
}



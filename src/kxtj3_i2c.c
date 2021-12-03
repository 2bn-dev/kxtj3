#include "kxtj3_i2c.h"
#include "kxtj3_registers.h"
#include "kxtj3_debug.h"

uint8_t kxtj3_i2c_target_address = KXTJ3_I2C_DEFAULT_ADDRESS;


bool kxtj3_get_bit(uint8_t const value, uint8_t const bit_number){ 
        return (value&(1<<bit_number)) >> bit_number;
}
uint8_t kxtj3_flip_bit(uint8_t const value, uint8_t const bit_number){ 
	return value ^ (1 << bit_number);
}


uint8_t kxtj3_get_i2c_target_address(){
	return kxtj3_i2c_target_address;
}

int kxtj3_set_i2c_target_address(uint8_t target_address){
	//Reserved I2C addresses
	if((target_address&0x78) == 0 || (target_address & 0x78) == 0x78)
		return I2C_INVALID_TARGET_ADDRESS;
	kxtj3_i2c_target_address = target_address;
	
	return I2C_SUCCESS;
}


#include "kxtj3_i2c_rp2040.h"
#include "kxtj3_debug.h"


void kxtj3_i2c_init_local(){
	_DBG("i2c_init()");
	i2c_init(I2C_INTERFACE, I2C_SPEED);
	gpio_set_function(PIN_I2C_SDA, GPIO_FUNC_I2C);
	gpio_set_function(PIN_I2C_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(PIN_I2C_SDA);
	gpio_pull_up(PIN_I2C_SCL);

	//Tell PICOTOOL about I2C pins
	_DBG("i2c_init() complete");
}

int kxtj3_i2c_read_register_blocking_uint8(uint8_t const target_addr, uint8_t const register_addr, uint8_t length){
	_DBG("blocking register read target_addr: 0x%02x, register_addr: 0x%02x, rbxuf size: %d", target_addr, register_addr, length);

	if(length > I2C_BUF_SIZE)
		return I2C_RXBUF_TOO_SMALL;

	//Reserved I2C addresses
	if((target_addr&0x78) == 0 || (target_addr & 0x78) == 0x78) 
		return I2C_INVALID_TARGET_ADDRESS;
	
	if(!kxtj3_valid_read_address(register_addr)) 
		return I2C_INVALID_REGISTER_ADDRESS;

	int ret = 0;
	ret = i2c_write_blocking_until(I2C_INTERFACE, target_addr, &register_addr, sizeof(register_addr), true, make_timeout_time_ms(I2C_WRITE_TIMEOUT_MS));
	if(ret == PICO_ERROR_TIMEOUT) return I2C_WRITE_TIMEOUT;
	if(ret == PICO_ERROR_GENERIC) return I2C_ADDR_DIDNT_ACK;
	if(ret < 1) return I2C_ERROR_UNKNOWN;

	memset(rxbuf, 0x0, I2C_BUF_SIZE);
	ret = i2c_read_blocking_until(I2C_INTERFACE, target_addr, rxbuf, length, false, make_timeout_time_ms(I2C_READ_TIMEOUT_MS));
	if(ret == PICO_ERROR_TIMEOUT) return I2C_WRITE_TIMEOUT;
	if(ret == PICO_ERROR_GENERIC) return I2C_ADDR_DIDNT_ACK;
	if(ret < 1) return I2C_ERROR_UNKNOWN;
	
	_DBG("Blocking read result: target_addr: 0x%02x, register_addr: 0x%02x, rbxuf size: %d, rxbuf: 0x%02x%02x%02x%02x%02x%02x%02x%02x ret: %d", target_addr, register_addr, length, rxbuf[0], rxbuf[1], rxbuf[2], rxbuf[3], rxbuf[4], rxbuf[5], rxbuf[6], rxbuf[7], ret);
	return ret; // Number of bytes read
}

int kxtj3_i2c_return_register_byte(uint8_t const register_addr){
	uint8_t length = 1;
	int ret = kxtj3_i2c_read_register_blocking_uint8(kxtj3_get_i2c_target_address(), register_addr, length);
	_DBG("single byte read: %02x, result: %02x", register_addr, rxbuf[0]);

	if(ret < 1) 
		return ret;
	int result;
	result = rxbuf[0];
	return result;
}	

int kxtj3_i2c_write_register_blocking_uint8(uint8_t const target_addr, uint8_t const register_addr, uint8_t length){
	if(length > I2C_BUF_SIZE)
		return I2C_RXBUF_TOO_SMALL;
	_DBG("blocking register write target_addr: %02x, register_addr: %02x, tbxuf size: %d", target_addr, register_addr, length);

	//Reserved I2C addresses 
        if((target_addr&0x78) == 0 || (target_addr & 0x78) == 0x78){
                return I2C_INVALID_TARGET_ADDRESS;
        }

	if(!kxtj3_valid_write_address(register_addr)){
                return I2C_INVALID_REGISTER_ADDRESS;
        }

	int ret = 0;
	for(int i = I2C_BUF_SIZE; i >= 0; --i){
		txbuf[i] = txbuf[i-1];
	}
	txbuf[0] = register_addr;

	ret = i2c_write_blocking_until(I2C_INTERFACE, target_addr, txbuf, length+1, false, make_timeout_time_ms(I2C_WRITE_TIMEOUT_MS));
	memset(txbuf, 0x0, I2C_BUF_SIZE);
	if(ret == PICO_ERROR_TIMEOUT)
                return I2C_WRITE_TIMEOUT;
        if(ret == PICO_ERROR_GENERIC)
                return I2C_ADDR_DIDNT_ACK;
        if(ret < 1)
                return I2C_ERROR_UNKNOWN;
        

	return ret;
}

int kxtj3_i2c_set_register_bit_mode(bool mode, uint8_t register_address, uint8_t bit_position){
	int ret;
	uint8_t length = 1;
	
	ret = kxtj3_i2c_read_register_blocking_uint8(kxtj3_get_i2c_target_address(), register_address, length);
	if( ret < 1){
		return ret;
	}

	uint8_t current_mode = kxtj3_get_bit(rxbuf[0], bit_position);

	_DBG("Address: %02x, Position: %d, rxbuf: %02x, current_mode: %d desired mode: %d ", register_address, bit_position,  rxbuf[0], current_mode, mode);
	if(current_mode == mode){
		return current_mode;
	}
	txbuf[0] = kxtj3_flip_bit(rxbuf[0], bit_position);
	_DBG("Address: %02x, Position: %d, txbuf: %02x", register_address, bit_position, txbuf[0]);
	ret = kxtj3_i2c_write_register_blocking_uint8(kxtj3_get_i2c_target_address(), register_address, length);
	if(ret < 1){
		_DBG("WRITE ERROR %d", ret);
		return ret;
	}

	return mode;
}



#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/time.h"
#include "hardware/i2c.h"

#include <kxtj3.h>
#include <kxtj3_i2c.h>
#include <kxtj3_debug.h>

#define PIN_I2C_SDA 8
#define PIN_I2C_SCL 9
#include <kxtj3_i2c_rp2040.h>


// This was the original application used for development and initial testing.
// it will wait for a USB serial connection.

void fatal_error_infinite_loop(){
	printf("\nExecution Stalled\n");
	while(true){
		_DBG("FATAL ERROR STALL!");
		sleep_ms(1000);
	}
}

void fatal_error(const char * detail){
	printf("\n\nFATAL: %s!\n\n\n", detail);
	fatal_error_infinite_loop();
}

void warning(const char * detail){
	printf("\n\nWARNING: %s!\n\n\n", detail);
}

int main(){
	stdio_usb_init();
	while(!stdio_usb_connected()){
		sleep_ms(1);
	}
	_DBG("USB Connected");
	sleep_ms(100);
	kxtj3_i2c_init_local();

/*
	while(true){
		_DBG("%d", kxtj3_set_i2c_target_address(0xc));
		sleep_ms(100);
		_DBG("%d", kxtj3_set_srst_mode(true));
		sleep_ms(100);
		_DBG("%d", kxtj3_send_startup_command());
		_DBG("%d", kxtj3_set_i2c_target_address(0xd));
		sleep_ms(100);
		_DBG("%d", kxtj3_set_srst_mode(true));
		sleep_ms(100);
		_DBG("%d", kxtj3_send_startup_command());
		_DBG("%d", kxtj3_set_i2c_target_address(0xe));
		sleep_ms(100);
		_DBG("%d", kxtj3_set_srst_mode(true));
		sleep_ms(100);
		_DBG("%d", kxtj3_send_startup_command());
		_DBG("%d", kxtj3_set_i2c_target_address(0xf));
		sleep_ms(100);
		_DBG("%d", kxtj3_set_srst_mode(true));
		sleep_ms(100);
		_DBG("%d", kxtj3_send_startup_command());
		_DBG("%d", kxtj3_set_i2c_target_address(0xe));
		sleep_ms(100);
		_DBG("%d", kxtj3_send_startup_command());
	}
	*/


	if(kxtj3_send_startup_command() < 1)
		warning("KXTJ3 not responding? (startup command failed)");

	if(kxtj3_set_srst_mode(true) < 1)
		                fatal_error("KXTJ3 failed to soft reset");
	sleep_ms(200);

	if(!kxtj3_functionality_test())
		fatal_error("KXTJ3 Functionality test failed!");

	if(kxtj3_set_operating_mode(false) != false)
		fatal_error("KXTJ3 Failed to set operating mode = standby");

	if(kxtj3_set_resolution_mode(true) != true)
		fatal_error("KXtJ3 Failed to set resolution mode = high ");

	if(kxtj3_set_acceleration_range(KXTJ3_RANGE_16G_4) < 1)
		fatal_error("KXTJ3 Failed to set Accleration Range = 2G");

	if(kxtj3_set_output_data_rate(KXTJ3_ODR_12P500HZ) < 1)
		fatal_error("KXTJ3 Failed to set 12.5 Hz output data rate");

	if(kxtj3_set_operating_mode(true) != true)
		fatal_error("KXTJ3 Failed to set operating mode = operating");

	kxtj3_xyz_t *acceleration = (kxtj3_xyz_t*) malloc(sizeof(kxtj3_xyz_t)*2);
	while(true){
		if(kxtj3_get_xyz(acceleration, KXTJ3_BIT_DEPTH_14) < 1)
			fatal_error("KXTJ3 Failed to get coordinates");
		printf("X: %hd Y: %hd Z: %hd\n", acceleration->x, acceleration->y, acceleration->z);
		sleep_ms(100);
	}
	free(acceleration);
	free(kxtj3_i2c_get_rxbuf_ptr());
	free(kxtj3_i2c_get_txbuf_ptr());

	return 0;
}



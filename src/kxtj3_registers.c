#include "kxtj3_registers.h"

uint8_t KXJT3_VALID_READ_ADDRESSES[] = {
        KXTJ3_REG_XOUT_L,
        KXTJ3_REG_XOUT_H,
        KXTJ3_REG_YOUT_L,
        KXTJ3_REG_YOUT_H,
        KXTJ3_REG_ZOUT_L,
        KXTJ3_REG_ZOUT_H,
        KXTJ3_REG_DCST_RESP,
        KXTJ3_REG_WHO_AM_I,
        KXTJ3_REG_INT_SOURCE1,
        KXTJ3_REG_INT_SOURCE2,
        KXTJ3_REG_STATUS_REG,
        KXTJ3_REG_INT_REL,
        KXTJ3_REG_CTRL_REG1,
        KXTJ3_REG_CTRL_REG2,
        KXTJ3_REG_INT_CTRL_REG1,
        KXTJ3_REG_INT_CTRL_REG2,
        KXTJ3_REG_DATA_CTRL_REG,
        KXTJ3_REG_WAKEUP_COUNTER,
        KXTJ3_REG_NA_COUNTER,
        KXTJ3_REG_WAKEUP_THRESHOLD_H,
        KXTJ3_REG_WAKEUP_THRESHOLD_L
};

uint8_t KXJT3_VALID_WRITE_ADDRESSES[] = {
        KXTJ3_REG_CTRL_REG1,
        KXTJ3_REG_CTRL_REG2,
        KXTJ3_REG_INT_CTRL_REG1,
        KXTJ3_REG_INT_CTRL_REG2,
        KXTJ3_REG_DATA_CTRL_REG,
        KXTJ3_REG_WAKEUP_COUNTER,
        KXTJ3_REG_NA_COUNTER,
        KXTJ3_REG_SELF_TEST,
        KXTJ3_REG_WAKEUP_THRESHOLD_H,
        KXTJ3_REG_WAKEUP_THRESHOLD_L,
};


bool kxtj3_valid_read_address(uint8_t address){
	bool register_found = false;
        for(uint8_t i = 0; i < sizeof(KXJT3_VALID_READ_ADDRESSES); i++){
                if(KXJT3_VALID_READ_ADDRESSES[i] == address){
                        register_found = true;
                        break;
                }
        }

	return register_found;
}

bool kxtj3_valid_write_address(uint8_t address){
	bool register_found = false;
        for(uint8_t i = 0; i < sizeof(KXJT3_VALID_READ_ADDRESSES); i++){
                if(KXJT3_VALID_READ_ADDRESSES[i] == address){
                        register_found = true;
                        break;
                }
        }

        return register_found;
}


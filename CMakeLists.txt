cmake_minimum_required(VERSION 3.13)

if (PICO_SDK_PATH)
	project(kxtj3 C ASM)
	
	add_library(kxtj3 INTERFACE)

	target_sources(kxtj3 INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3.c
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3.h
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3_registers.c
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3_registers.h
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3_i2c.c
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3_i2c.h
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3_i2c_rp2040.c
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3_i2c_rp2040.h
		${CMAKE_CURRENT_LIST_DIR}/src/kxtj3_debug.h
		)
	target_include_directories( kxtj3 INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/src
		)

	return()
else()
	message("######################################################")
        message("Warning: this is a library, it can't be directly built")
	message("you need to include it into your own project (see examples/)")
        message("######################################################")
        message(FATAL_ERROR "Quitting configuration")

endif()

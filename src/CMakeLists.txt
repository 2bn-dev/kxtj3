
include(../pico_sdk_import.cmake)

project(kxtj3)

pico_sdk_init()

add_library(kxtj3 STATIC kxtj3.c )
add_library(kxtj3_registers STATIC kxtj3_registers.c )
add_library(kxtj3_i2c STATIC kxtj3_i2c.c)
add_library(i2c_rp2040 STATIC i2c_rp2040.c)

target_include_directories(kxtj3 PUBLIC ../include/kxtj3-1057)
target_include_directories(kxtj3_registers PUBLIC ../include/kxtj3-1057)
target_include_directories(kxtj3_i2c PUBLIC ../include/kxtj3-1057)
target_include_directories(i2c_rp2040 PUBLIC ../include/kxtj3-1057)



target_link_libraries(kxtj3_registers)
target_link_libraries(kxtj3_i2c)
target_link_libraries(i2c_rp2040 kxtj3_registers kxtj3_i2c pico_stdlib pico_stdio hardware_i2c)
target_link_libraries(kxtj3 LINK_PUBLIC i2c_rp2040)




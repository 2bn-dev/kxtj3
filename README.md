# KXTJ3-1057 Accelerometer C library

This library implements the API for accessing the Rohm / Kionix KXTJ3-1057 Accelerometer. It implements all interfaces in the datasheet. It is a low cost, 3-axis accelerometer with I2C communication as well as an interrupt interface. It supports 8 bit, 12 bit, and 14 bit depths, +/- 2G, 4G, 8G, or 16G ranges, and many output data rates from 0.781 Hz to 1600 Hz. It provides extreme low power modes with wake-up interrupt configuration settings.

## Supported devices
 * RP2040 (via C pico-sdk hardware/i2c.h)
 * [planned] ESP32 (via ESP-IDF)

As these are the only two devices I have with this acceleromter at the moment that is all I will be able to do right now. If you would like to provide different hardware with access to this accelerometer I will consider your hardware for inclusion.

## Known Caveats

 * I haven't tested all possible combinations of modes, rates, and ranges
 * My devices don't use the interrupt pin, so I am unable to test that functionality

Please report any bugs in Github issues and I will do my best to address them.

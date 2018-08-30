# avr-twi-master-driver

Nonblocking TWI/I2C master driver for Atmel AVR.

Test configured for Atmega 2560.

## Setup

Driver can be found in: 
* `twi_test/twi_test/twi_drv.h`
* `twi_test/twi_test/twi_drv.c`

In order to use the driver, you also need to copy the file
`twi_test/twi_test/boardconfig.h` and set `F_CPU` to the correct value.

If you do not want to copy the `boardconfig.h`, delete the include in `twi_drv.c` and define `F_CPU` somewhere else.

## API

###### `void twi_init()`

Initializes TWI. Should be called once before calling any other TWI functions.

Interrupts need to be enabled externally via `sei()` **BEFORE** initialization.

###### `uint8_t twi_beginRead(uint8_t address, uint8_t length)`

Begin reading from address (a specific amount of bytes).

Data can be read by calling `twi_getReceivedData()` as soon as `twi_isBusy()` returns 0.

*params:*
* address - address to read from
* length - amount of bytes to read

*returns:* 1 on success, 0 if twi is busy (another transmission is running)

###### `uint8_t twi_beginWrite(uint8_t address, uint8_t* data, uint8_t length)`

Begin writing to address.

*params:*
* address - address to write to
* data - pointer to array of data-bytes
* length - amount of bytes in data-array (amount of bytes to write)

*returns:* 1 on success, 0 if twi is busy (another transmission is running)

###### `uint8_t twi_isBusy()`

Check if TWI is busy. Busy means that a transmission is running.

*returns:* 1 if twi is busy, 0 otherwise

###### `uint8_t twi_getReceivedData(uint8_t* outAddress, uint8_t* outData, uint8_t* outLength)`

Get received data from last transmission (if it was a read operation).

*params:*
* outAddress - output pointer for read address, can be null
* outData - output pointer to array for data-bytes
* outLength - output pointer for read amount of bytes, can be null

*returns:* 1 on success, 0 if twi is busy or last transmission was not a read operation.

## Definitions

* `F_CPU` - you should define this before including this library (read "Setup")
* `TWI_FREQ` - defaults to 100kHz
* `TWI_BUFFER_SIZE` - defaults to 32

## Example

Look at the main.c from twi_test.

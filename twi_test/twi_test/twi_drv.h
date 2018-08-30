/** 
 * @author Alexander Droste
 * date: 30.08.18
 * based on https://github.com/scttnlsn/avr-twi
 */

#ifndef TWI_DRV_H_
#define TWI_DRV_H_

#include <avr/io.h>

#define TWI_FREQ		100000UL // 100kHz
#define TWI_BUFFER_SIZE 32


/**
 * Initializes TWI.
 * Interrupts need to be enabled externally via sei() >>BEFORE<< initialization.
 */
void twi_init();


/** 
 * Begin reading from address (a specific amount of bytes).
 * Data can be read by calling twi_getTransmissionData() as soon as twi_isBusy() returns 0.
 * @param address address to read from
 * @param length amount of bytes to read (max TWI_BUFFER_SIZE - 1)
 * @returns 1 on success, 0 if twi is busy (another transmission is running)
 */
uint8_t twi_beginRead(uint8_t address, uint8_t length);


/** 
 * Begin writing to address.
 * @param address address to write to
 * @param data pointer to array of data-bytes
 * @param length amount of bytes in data-array (amount of bytes to write)
 * @returns 1 on success, 0 if twi is busy (another transmission is running)
 */
uint8_t twi_beginWrite(uint8_t address, uint8_t* data, uint8_t length);


/** 
 * Check if TWI is busy. Busy means that a transmission is running.
 * @returns 1 if twi is busy, 0 otherwise
 */
uint8_t twi_isBusy();



/** 
 * Get received data from last transmission (if it was a read operation).
 * @param outAddress output pointer for read address, can be null
 * @param outData output pointer to array for data-bytes
 * @param outLength output pointer for read amount of bytes, can be null
 * @param 1 on success, 0 if twi is busy or last transmission was not a read operation.
 */
uint8_t twi_getReceivedData(uint8_t* outAddress, uint8_t* outData, uint8_t* outLength);


#endif /* TWI_DRV_H_ */
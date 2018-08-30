/**
 * @author Alexander Droste
 * date: 30.08.18
 * based on https://github.com/scttnlsn/avr-twi
 */

#include <avr/interrupt.h>
#include <util/twi.h>
#include <string.h>
#include "twi_drv.h"
#include "boardconfig.h"

// =====================================
// private defines & data
// =====================================

static volatile uint8_t _isBusy;
static struct {
	uint8_t buffer[TWI_BUFFER_SIZE];
	uint8_t length;
	uint8_t index;
} _transmission;



// =====================================
// private interface
// =====================================

/** 
 * Send START.
 * @private
 */
void _start() {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWSTA);
}


/** 
 * Send STOP.
 * @private
 */
void _stop() {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}


/** 
 * Send ACK.
 * @private
 */
void _ack() {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
}


/** 
 * Send NOT ACK.
 * @private
 */
void _nack() {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
}


/** 
 * Write data to TWDR.
 * @private
 * @param data data-byte to write
 */
void _send(uint8_t data) {
	TWDR = data;
}


/** 
 * Read in data from TWDR and save it in transmission buffer.
 * @private
 */
void _receive() {
	_transmission.buffer[_transmission.index++] = TWDR;
}


/** 
 * Send ACK or NACK depending on transmission process.
 * @private
 */
void _reply() {
	if (_transmission.index < (_transmission.length - 1)) {
		_ack();
	} else {
		_nack();
	}
}


/**
 * TWINT interrupt handler.
 * Controls TWI transmission.
 */
ISR(TWI_vect) {
	switch (TW_STATUS) {
		case TW_START:
		case TW_REP_START:
		case TW_MT_SLA_ACK:
		case TW_MT_DATA_ACK:
			if (_transmission.index < _transmission.length) {
				_send(_transmission.buffer[_transmission.index++]);
				_nack();
			} else {
				_stop();
				_isBusy = 0;
			}
			break;

		case TW_MR_DATA_ACK:
			_receive();
			_reply();
			break;

		case TW_MR_SLA_ACK:
			_reply();
			break;

		case TW_MR_DATA_NACK:
			_receive();
			_stop();
			_isBusy = 0;
			break;

		case TW_MT_SLA_NACK:
		case TW_MR_SLA_NACK:
		case TW_MT_DATA_NACK:
		default:
			_stop();
			_isBusy = 0;
			break;
	}
}



// =====================================
// public interface
// =====================================

void twi_init() {
	TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
	TWSR = 0; // prescaler = 1
	_isBusy = 0;
	TWCR = 1<<TWEN;
}


uint8_t twi_beginRead(uint8_t address, uint8_t length) {
	if (_isBusy)
		return 0;

	_isBusy = 1;
	_transmission.buffer[0] = (address << 1) | TW_READ;
	_transmission.length = length + 1;
	_transmission.index = 0;
	_start();
	return 1;
}


uint8_t twi_beginWrite(uint8_t address, uint8_t* data, uint8_t length) {
	if (_isBusy)
		return 0;

	_isBusy = 1;
	_transmission.buffer[0] = (address << 1) | TW_WRITE;
	_transmission.length = length + 1;
	_transmission.index = 0;
	memcpy(&_transmission.buffer[1], data, length);
	_start();
	return 1;
}


uint8_t twi_isBusy() {
	return _isBusy;
}


uint8_t twi_getReceivedData(uint8_t* outAddress, uint8_t* outData, uint8_t* outLength) {
	if (_isBusy || (_transmission.buffer[0] & 0x1) != TW_READ)
		return 0;

	if (outAddress)
		*outAddress = _transmission.buffer[0] >> 1;
    if (outLength)
		*outLength = _transmission.length - 1;
	memcpy(outData, &_transmission.buffer[1], _transmission.length - 1);
	return 1;
}

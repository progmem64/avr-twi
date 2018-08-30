/*
 * twi_test.c
 *
 * Created: 29.08.2018 19:14:38
 * Author : tiws-16-4
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi_drv.h"


int main(void)
{
	sei();
	twi_init();
	
	uint8_t data[4] = { 0x11, 0x22, 0x44, 0x88 };
	twi_beginWrite(0x44, data, 4);
	while (twi_isBusy());

	twi_beginRead(0x44, 4);
	while (twi_isBusy());
	
	uint8_t address;
	uint8_t rxdat[4];
	uint8_t length;
	twi_getReceivedData(&address, rxdat, &length);
	
	while(1) {}
}


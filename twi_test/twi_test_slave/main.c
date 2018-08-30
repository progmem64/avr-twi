/*
 * twi_test_slave.c
 *
 * Created: 29.08.2018 20:47:30
 * Author : tiws-16-4
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "I2CSlave.h"


volatile uint8_t data[4] = { 0xFF, 0xFF, 0xFF, 0xFF } ;
volatile uint8_t widx = 0, ridx = 0;

void I2C_received(uint8_t received_data)
{
	data[widx++] = received_data;
	if (widx >= 4)
		widx = 0;
}

void I2C_requested()
{
	I2C_transmitByte(data[ridx++]);
	if (ridx >= 4)
		ridx = 0;
}


int main(void)
{
	I2C_setCallbacks(I2C_received, I2C_requested);
	I2C_init(0x44);
	sei();

	DDRH = 0xFF;
	DDRE = 0xFF;
	DDRK = 0xFF;
	DDRA = 0xFF;
	
    while (1) 
    {
		PORTH = data[0];
		PORTE = data[1];
		PORTK = data[2];
		PORTA = data[3];
    }
}


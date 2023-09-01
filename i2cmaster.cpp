/*
 * i2cMaster.c
 *
 *  Created on: 27 sty 2023
 *      Author: adminW
 */


#include <avr/io.h>
#include "i2cMaster.h"
#include <util/delay.h>

void I2C_init( void )
{
	TWSR &= ~_BV( TWPS1 );
	TWSR &= ~_BV( TWPS0 );
	TWBR = 47;
}

void I2C_start( void )
{
	TWCR = _BV( TWINT ) | _BV( TWSTA ) | _BV( TWEN );
	I2C_waitForComplete();
}

void I2C_stop( void )
{
	TWCR = _BV( TWINT ) | _BV( TWEN ) | _BV( TWSTO );
	while( TWCR & _BV(TWSTO));
}

void I2C_stopNoWait( void )
{
	TWCR = _BV( TWINT ) | _BV( TWEN ) | _BV( TWSTO );
}

void I2C_transmitData( uint8_t data )
{
	TWDR = data;
	TWCR = _BV( TWINT ) | _BV( TWEN );
	I2C_waitForComplete();
}

uint8_t I2C_receiveData_ACK( void )
{
	TWCR = _BV( TWEA ) | _BV( TWINT ) | _BV( TWEN );
	I2C_waitForComplete();
	return TWDR;
}

uint8_t I2C_receiveData_NACK( void )
{
	TWCR = _BV( TWINT ) | _BV( TWEN );
	I2C_waitForComplete();
	return TWDR;
}

void I2C_startAndTransmitAddress( uint8_t address )
{
	I2C_start();
	I2C_transmitData( address );
}

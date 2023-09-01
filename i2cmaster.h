/*
 * i2cMaster.h
 *
 *  Created on: 27 sty 2023
 *      Author: adminW
 */

#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#include <avr/io.h>
#include <util/twi.h>

#define I2C_WRITE	0
#define I2C_READ	1

static inline void I2C_waitForComplete( void )
{
	while( !(TWCR & _BV(TWINT)));
};

void I2C_init( void );
void I2C_start( void );
void I2C_stop( void );
void I2C_stopNoWait( void );
void I2C_transmitData( uint8_t data );
void I2C_startAndTransmitAddress( uint8_t address );
uint8_t I2C_receiveData_ACK( void );
uint8_t I2C_receiveData_NACK( void );


#endif /* I2CMASTER_H_ */

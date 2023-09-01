/*
 * usart.h
 *
 *  Created on: 27 sty 2023
 *      Author: adminW
 */

#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

#define RESOLUTION 35

#define USART_STATE_DDR		DDRB
#define USART_STATE_PORT	PORTB
#define USART_STATE_PIN		PINB

#define USART_STATE			_BV( PB0 )

void USART_init( void );
void USART_transmitData( uint8_t data );
void USART_transmitString( char *s );
void USART_displayVoltage( uint16_t voltageDEC, uint32_t voltageFRA, uint8_t range, uint8_t sign );
void USART_displayRange( uint8_t range );
char *myItoa( uint32_t x, char s[], uint8_t n );

#define USART_ONE		    USART_transmitString( "1[" )


#endif /* USART_H_ */

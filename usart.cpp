/*
 * usart.c
 *
 *  Created on: 27 sty 2023
 *      Author: adminW
 */

#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUF_SIZE	10

void USART_init( void )
{
	UBRR0 = 71;
	UCSR0B |=  _BV( TXEN0 );
	UCSR0C &= ~( _BV( UMSEL01 ) | _BV( UMSEL00 ) ); // tryb asynchroniczny
	UCSR0C &= ~( _BV( UPM01 ) | _BV( UPM00 ) ); // wylaczony bit parzystosci
	UCSR0C &= ~_BV( USBS0 ); // 1 bit stopu
	UCSR0C |= ( _BV( UCSZ01 ) | _BV( UCSZ00 ) ); // interfejs 8-bit
	USART_STATE_DDR &= ~USART_STATE;
}

void USART_transmitData( uint8_t data )
{
	while( !(UCSR0A & _BV(UDRE0)) );
	UDR0 = data;
}

void USART_transmitString( char *s )
{
	while( *s )
		USART_transmitData( *s++ );
}

void USART_displayVoltage( uint16_t voltageDEC, uint32_t voltageFRA, uint8_t range, uint8_t sign )
{
  char buf[ BUF_SIZE ];

  if( ((range == 1) && (voltageDEC >= 200)) || ((range == 2) && (voltageDEC >= 2)) || ((range == 3) && (voltageDEC >= 20)) ){
    USART_ONE;
    return;
  }

  if( sign == 0 ) USART_transmitString( "-" );

  switch( range ){
    case 1:
        #if RESOLUTION == 35
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " mV" );
        #elif RESOLUTION == 45
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          if( voltageFRA < 10 ) USART_transmitString( "0" );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " mV" );
        #else
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          if( voltageFRA < 10 ) USART_transmitString( "00" );
          else if( voltageFRA < 100 ) USART_transmitString( "0" );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " mV" );
        #endif
      break;
    case 2:
        #if RESOLUTION == 35
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          if( voltageFRA < 10 ) USART_transmitString( "00" );
          else if( voltageFRA < 100 ) USART_transmitString( "0" );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " V" );
        #elif RESOLUTION == 45
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          if( voltageFRA < 10 ) USART_transmitString( "000" );
          else if( voltageFRA < 100 ) USART_transmitString( "00" );
          else if( voltageFRA < 1000 ) USART_transmitString( "0" );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " V" );
        #else
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          if( voltageFRA < 10 ) USART_transmitString( "0000" );
          else if( voltageFRA < 100 ) USART_transmitString( "000" );
          else if( voltageFRA < 1000 ) USART_transmitString( "00" );
          else if( voltageFRA < 10000 ) USART_transmitString( "0" );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " V" );
        #endif
      break;     
    case 3:
        #if RESOLUTION == 35
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          if( voltageFRA < 10 ) USART_transmitString( "0" );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " V" );
        #elif RESOLUTION == 45
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          if( voltageFRA < 10 ) USART_transmitString( "00" );
          else if( voltageFRA < 100 ) USART_transmitString( "0" );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " V" );
        #else
          USART_transmitString( myItoa(voltageDEC, buf, BUF_SIZE) );
          USART_transmitString( "," );
          if( voltageFRA < 10 ) USART_transmitString( "000" );
          else if( voltageFRA < 100 ) USART_transmitString( "00" );
          else if( voltageFRA < 1000 ) USART_transmitString( "0" );
          USART_transmitString( myItoa(voltageFRA, buf, BUF_SIZE) );
          USART_transmitString( " V" );
        #endif
      break;
  }


  USART_transmitString( "[" );
}

void USART_displayRange( uint8_t range )
{
  if( range == 1 ) USART_transmitString("Zakres 200 mV");
  else if( range == 2 ) USART_transmitString("Zakres 2 V");
  else if( range == 3 ) USART_transmitString("Zakres 20 V");

  USART_transmitString( "]" );
}

char *myItoa( uint32_t x, char s[], uint8_t n )
{
	uint8_t i, j, k;
	uint8_t tmp;

	if( x == 0 ){
		s[ 0 ] = 0 + '0';
		s[ 1 ] = '\0';
		return s;
	}

	i = 0;
	while( x && (i < n) ){
		s[ i++ ] = (x % 10) + '0';
		x = x / 10;
	}

	for( j = 0, k = i-1; j < i/2; j++, k-- ){
		tmp = s[ j ];
		s[ j ] = s[ k ];
		s[ k ] = tmp;
	}

	s[ i ] = '\0';
	return s;
}

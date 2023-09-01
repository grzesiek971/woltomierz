/*
 * ads1115.c
 *
 *  Created on: 25 lut 2023
 *      Author: adminW
 */

#include "ads1115.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2cMaster.h"

volatile int16_t adc;
volatile uint8_t range;
volatile uint8_t filterIndex;
volatile uint16_t filterAverage;
volatile uint32_t filterSum;
volatile uint16_t filterArray[ FILTER_LENGTH ];
volatile uint8_t sign;


void ADS_init( void )
{
  DDRD &= ~ADS_READY;
  EICRA = _BV( ISC01 );
	EIMSK = _BV( INT0 );

	ADS_RANGE_DDR &= ~( RANGE_200_MV | RANGE_20_V | RANGE_2_V );
	ADS_RANGE_PORT |= RANGE_200_MV | RANGE_20_V | RANGE_2_V;

  I2C_init();

	I2C_startAndTransmitAddress( (ADS_ADDRESS_GND << 1) + I2C_WRITE );
	I2C_transmitData( 0x03 );
	I2C_transmitData( (uint8_t)(0x8000 >> 8) );
	I2C_transmitData( (uint8_t)(0x8000 & 0x00FF) );
	I2C_stop();

	I2C_startAndTransmitAddress( (ADS_ADDRESS_GND << 1) + I2C_WRITE );
	I2C_transmitData( 0x02 );
	I2C_transmitData( (uint8_t)(0x7FFF >> 8) );
	I2C_transmitData( (uint8_t)(0x7FFF & 0x00FF) );
	I2C_stop();
}

void ADS_startConversion( uint8_t range )
{
	uint16_t config;

	if( range == 1 ){
	    config =  ADS_AIN0_AIN3 |
	              ADS_FSR_0_256 |
	              ADS_MODE_CONTINUOUS |
	              ADS_DR_128_SPS |
	              ADS_COMP_MODE_TRADITIONAL |
	              ADS_COMP_POL_LOW |
	              ADS_COMP_NON_LATCHING |
	              ADS_COMP_QUE_ONE;
	  }
	  else if( range == 2 ){
	    config =  ADS_AIN0_AIN3 |
	              ADS_FSR_2_048 |
	              ADS_MODE_CONTINUOUS |
	              ADS_DR_128_SPS |
	              ADS_COMP_MODE_TRADITIONAL |
	              ADS_COMP_POL_LOW |
	              ADS_COMP_NON_LATCHING |
	              ADS_COMP_QUE_ONE;
	  }
	  else if( range == 3 ){
	    config =  ADS_AIN1_AIN3 |
	              ADS_FSR_2_048 |
	              ADS_MODE_CONTINUOUS |
	              ADS_DR_128_SPS |
	              ADS_COMP_MODE_TRADITIONAL |
	              ADS_COMP_POL_LOW |
	              ADS_COMP_NON_LATCHING |
	              ADS_COMP_QUE_ONE;
	 }

	I2C_startAndTransmitAddress( (ADS_ADDRESS_GND << 1) + I2C_WRITE );
	I2C_transmitData( 0x01 );
	I2C_transmitData( (uint8_t)(config >> 8) );
	I2C_transmitData( (uint8_t)(config & 0x00FF) );
	I2C_stop();
}

int16_t ADS_read( void )
{
	uint8_t adcH, adcL;
	uint16_t adc;

	I2C_startAndTransmitAddress( (ADS_ADDRESS_GND << 1) + I2C_WRITE );
	I2C_transmitData( 0x00 );
	I2C_stop();

	I2C_startAndTransmitAddress( (ADS_ADDRESS_GND << 1) + I2C_READ );
	adcH = I2C_receiveData_ACK();
	adcL = I2C_receiveData_NACK();
	I2C_stop();

	adc = (adcH << 8);
	adc |= adcL;

	return (int16_t)adc;
}

uint32_t ADS_voltageRoundUp( uint32_t voltage, uint8_t range )
{
	uint32_t i = 100;
    #if RESOLUTION == 35
	    if( range == 1 ) voltage /= 100000;
	    else voltage /= 1000;
    #elif RESOLUTION == 45
      if( range == 1 ) voltage /= 10000;
      else voltage /= 100;
    #else
      if( range == 1 ) voltage /= 1000;
      else voltage /= 10;
    #endif

	if( (voltage % 10) >= 5 ){
		while( (i <= 1000000) && ((voltage % i) == 9) )
			i *= 10;
		voltage += (i/10);
	}

	return (voltage/10);
}

ISR( INT0_vect )
{
  static uint8_t lastRange = 100;
  adc = ADS_read();
  if( adc < 0 ){
    adc = -adc;
    sign = 0;
  }
  else sign = 1;

  /* Średnia krocząca */
  // odjęcie ostatniego wyniku pomiaru
  filterSum = filterSum - filterArray[ filterIndex ];
  // przypisanie do tablicy obecnego wyniku pomiaru
  filterArray[ filterIndex ] = adc;
  // zsumowanie ostatnich pomiarów
  filterSum += adc;
  // Przesunięcie indexu tablicy w oczekiwaniu na kolejny pomiar
  filterIndex = (filterIndex + 1) % FILTER_LENGTH;
  // obliczenie średniej ostatnich pomiarów 
  filterAverage = filterSum / FILTER_LENGTH;  

  if( range != lastRange ){
    ADS_startConversion( range );
    lastRange = range;
  }
}

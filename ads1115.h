/*
 * ads1115.h
 *
 *  Created on: 29 sty 2023
 *      Author: adminW
 */

#ifndef ADS1115_H_
#define ADS1115_H_

#include <avr/io.h>
#define FILTER_LENGTH   100

extern volatile int16_t adc;
extern volatile uint8_t range;
extern volatile uint8_t filterIndex;
extern volatile uint16_t filterAverage;
extern volatile uint32_t filterSum;
extern volatile uint16_t filterArray[ FILTER_LENGTH ];
extern volatile uint8_t sign;

#define ADS_RANGE_DDR	DDRB
#define ADS_RANGE_PORT	PORTB
#define ADS_RANGE_PIN	PINB

#define RANGE_200_MV	_BV( PB4 )
#define RANGE_2_V		  _BV( PB3 )
#define RANGE_20_V		_BV( PB2 )
#define ADS_READY     _BV( PD2 )

#define LSB_R1	78125
#define LSB_R2	625

#define I2C_READ	1
#define I2C_WRITE	0

#define ADS_ADDRESS_GND		0b1001000
#define ADS_ADDRESS_VDD		0b1001001
#define ADS_ADDRESS_SDA		0b1001010
#define ADS_ADDRESS_SCL		0b1001011

#define ADS_START_CONVERSION	0x8000
#define ADS_BUSY				0x8000

#define ADS_AIN0_AIN1	0x0000	// default
#define ADS_AIN0_AIN3	0x1000
#define ADS_AIN1_AIN3	0x2000
#define ADS_AIN2_AIN3	0x3000
#define ADS_AIN0_GND	0x4000
#define ADS_AIN1_GND	0x5000
#define ADS_AIN2_GND	0x6000
#define ADS_AIN3_GND	0x7000

#define ADS_FSR_6_144	0x0000
#define ADS_FSR_4_096	0x0200
#define ADS_FSR_2_048	0x0400	// default
#define ADS_FSR_1_024	0x0600
#define ADS_FSR_0_512	0x0800
#define ADS_FSR_0_256	0x0A00

#define ADS_MODE_SINGLE			0x0100	// default
#define ADS_MODE_CONTINUOUS		0x0000

#define ADS_DR_8_SPS	0x0000
#define ADS_DR_16_SPS	0x0020
#define ADS_DR_32_SPS	0x0040
#define ADS_DR_64_SPS	0x0060
#define ADS_DR_128_SPS	0x0080	// default
#define ADS_DR_250_SPS	0x00A0
#define ADS_DR_475_SPS	0x00C0
#define ADS_DR_860_SPS	0x00E0

#define ADS_COMP_MODE_TRADITIONAL	0x0000	// default
#define ADS_COMP_MODE_WINDOW		0x0010

#define ADS_COMP_POL_LOW	0x0000	// default
#define ADS_COMP_POL_HIGH	0x0008

#define ADS_COMP_NON_LATCHING	0x0000	// default
#define ADS_COMP_LATCHING		0x0004

#define ADS_COMP_QUE_ONE	0x0000
#define ADS_COMP_QUE_TWO	0x0001
#define ADS_COMP_QUE_FOUR	0x0002
#define ADS_COMP_DISABLE	0x0003	// default

void ADS_init( void );
int16_t ADS_read( void );
uint32_t ADS_voltageRoundUp( uint32_t voltage, uint8_t range );
void ADS_startConversion( uint8_t range );

#endif /* ADS1115_H_ */

#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define NOKIA_SCLK  3
#define NOKIA_DIN   4
#define NOKIA_DC    5
#define NOKIA_CS    6
#define NOKIA_RST   7

extern Adafruit_PCD8544 nokia;

void NOKIA_init( void );
void NOKIA_displayVoltage( uint16_t napiecieCAL, uint32_t napiecieULAM, uint8_t zakres, uint8_t znak );
void NOKIA_displayRange( uint8_t zakres );
void NOKIA_transmitOne( void );
void NOKIA_bluetooth( uint8_t blu );

#endif

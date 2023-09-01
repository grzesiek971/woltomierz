
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "nokia5110.h"
#include "usart.h"

Adafruit_PCD8544 nokia = Adafruit_PCD8544( NOKIA_SCLK, NOKIA_DIN, NOKIA_DC, NOKIA_CS, NOKIA_RST );

static const uint8_t bluetoothIcon [] PROGMEM = {
	0x00, 0x00, 0x0c, 0x00, 0x0e, 0x00, 0x0b, 0x00, 0x08, 0xc0, 0x6b, 0x00, 0x3e, 0x00, 0x1c, 0x00, 
	0x3e, 0x00, 0x6b, 0x00, 0xc9, 0xc0, 0x0b, 0x80, 0x0e, 0x00, 0x0c, 0x00, 0x08, 0x00
};

void NOKIA_init( void )
{
  nokia.begin();
  nokia.clearDisplay();

  nokia.setContrast( 50 );
  nokia.setTextColor( BLACK );
  nokia.print( "START" );
  nokia.display();

  delay( 2000 );
}


void NOKIA_displayRange( uint8_t range )
{
  nokia.setCursor( 0,40 );
  nokia.setTextSize( 1 );
  if( range == 1 ) nokia.print( "Zakres 200 mV" );
  else if( range == 2 ) nokia.print( "Zakres 2 V" );
  else if( range == 3 ) nokia.print( "Zakres 20 V" );
}

void NOKIA_transmitOne( void )
{
  nokia.setCursor( 10,20 );
  nokia.setTextSize( 1 );
  nokia.setTextColor( BLACK );
  nokia.print( "1" );
}

void NOKIA_displayVoltage( uint16_t voltageDEC, uint32_t voltageFRA, uint8_t range, uint8_t sign )
{
  uint8_t x, y;

  if( ((range == 1) && (voltageDEC >= 200)) || ((range == 2) && (voltageDEC >= 2)) || ((range == 3) && (voltageDEC >= 20)) ){
    NOKIA_transmitOne();
    return;
  }

  nokia.setTextColor( BLACK );
  nokia.setCursor( 10, 20 );

  if( sign == 0 ) nokia.print( "-" );

  switch( range ){
    case 1:
        #if RESOLUTION == 35
          nokia.print( voltageDEC );
          nokia.print( "," );
          nokia.print( voltageFRA );
          nokia.print( " mV" );
        #elif RESOLUTION == 45
          nokia.print( voltageDEC );
          nokia.print( "," );
          if( voltageFRA < 10 ) nokia.print( "0" );
          nokia.print( voltageFRA );
          nokia.print( " mV" );
        #else
          nokia.print( voltageDEC );
          nokia.print( "," );
          if( voltageFRA < 10 ) nokia.print( "00" );
          else if( voltageFRA < 100 ) nokia.print( "0" );
          nokia.print( voltageFRA );
          nokia.print( " mV" );
        #endif
      break;
    case 2:
      #if RESOLUTION == 35
        nokia.print( voltageDEC );
        nokia.print( "," );
        if( voltageFRA < 10 ) nokia.print( "00" );
        else if( voltageFRA < 100 ) nokia.print( "0" );
        nokia.print( voltageFRA );
        nokia.print( " V" );
      #elif RESOLUTION == 45
        nokia.print( voltageDEC );
        nokia.print( "," );
        if( voltageFRA < 10 ) nokia.print( "000" );
        else if( voltageFRA < 100 ) nokia.print( "00" );
        else if( voltageFRA < 1000 ) nokia.print( "0" );
        nokia.print( voltageFRA );
        nokia.print( " V" );
      #else
        nokia.print( voltageDEC );
        nokia.print( "," );
        if( voltageFRA < 10 ) nokia.print( "0000" );
        else if( voltageFRA < 100 ) nokia.print( "000" );
        else if( voltageFRA < 1000 ) nokia.print( "00" );
        else if( voltageFRA < 10000 ) nokia.print( "0" );
        nokia.print( voltageFRA );
        nokia.print( " V" );
      #endif
      break;
    case 3:
        #if RESOLUTION == 35
          nokia.print( voltageDEC );
          nokia.print( "," );
          if( voltageFRA < 10 ) nokia.print( "0" );
          nokia.print( voltageFRA );
          nokia.print( " V" );
        #elif RESOLUTION == 45
          nokia.print( voltageDEC );
          nokia.print( "," );
          if( voltageFRA < 10 ) nokia.print( "00" );
          else if( voltageFRA < 100 ) nokia.print( "0" );
          nokia.print( voltageFRA );
          nokia.print( " V" );
        #else
          nokia.print( voltageDEC );
          nokia.print( "," );
          if( voltageFRA < 10 ) nokia.print( "000" );
          else if( voltageFRA < 100 ) nokia.print( "00" );
          else if( voltageFRA < 1000 ) nokia.print( "0" );
          nokia.print( voltageFRA );
          nokia.print( " V" );
        #endif
      break;
  }
}

void NOKIA_bluetooth( uint8_t blu )
{
  if( blu ) nokia.drawBitmap( 70, 0, bluetoothIcon, 10, 15, BLACK );
}


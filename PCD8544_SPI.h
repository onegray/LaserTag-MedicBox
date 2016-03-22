/********************************************************************
 This is a "Fast PCD8544 Library". It is designed to be used with
 Nokia 5110 type of display, driven by the PCD8544 controller.
 This library uses hardware SPI of your Arduino microcontroller,
 and does not supprt 'software SPI' mode.
 
 Written by Arthur Liberman (aka 'The Coolest'). http://www.alcpu.com
 Special thanks goes out to 'robtillaart' for his help with debugging
 and optimization.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
********************************************************************/

/*
 Software License Agreement (BSD License)
 
 Copyright (c) 2013, ALCPU (Arthur Liberman).  All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holders nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Version 1.2 -- Sept. 15, 2013

#pragma once
#include "Arduino.h"

// It is very important to wire the LCD correctly. 
// The following is the default wiring configuration for an Atmega168/328 based Arduino:
// PIN_DC    D8 
// PIN_RESET D9 
// PIN_SCE   D10 --  Not used in our version -- SergeyN
// PIN_SDIN  D11 
// PIN_SCLK  D13 

// You may change pins DC, Reset and SCE to map them to different pins on your Arduino board. 
// Please keep SDIN and SCLK connected to your Arduino's MOSI and SCK (hardware SPI) pins.
// You can remap the LCD control pins by changing the following '#define' values:
// Please refer to the pin/port mapping for your Arduino device:
// http://arduino.cc/en/Hacking/PinMapping168
// http://arduino.cc/en/Hacking/PinMapping2560
// Pins labeled Px (PB, PC, etc.) stand for PORTx.
// The current version of this library only allows the use of one port for the control pins.
// Pins are mapped in binary order. Valid values are 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80.
// Px0 is the LSB pin of the port, which in turn is represented by the hexadecimal number 0x01. 
// Px7 is the MSB pin of the port, which in turn is represented by the hexadecimal number 0x80.

// This is the port where the LCD is connected.
#define PCD8544_PORT		PORTB
#define PCD8544_DDR			DDRB	// Should be DDRx, x = port name (B, C, D, etc.)
						
#define PIN_DC				0x01	// D8
#define PIN_RESET			0x02	// D9
//#define PIN_CE			0x04	// D10
#define PIN_CE				0x0		// Do not need PIN_CE in our version -- SergeyN
#define PINS_CE_DC			(PIN_DC | PIN_CE)

// When DC is '1' the LCD expects data, when it is '0' it expects a command.
#define PCD8544_COMMAND		0 
#define PCD8544_DATA		PIN_DC

// You may find a different size screen, but this one is 84 by 48 pixels
#define PCD8544_X_PIXELS	84
#define PCD8544_Y_PIXELS	48
#define PCD8544_ROWS		6

#define BUF_LEN				504 // 84 * 6 (6 rows of 8 bits)

// Functions gotoXY, writeBitmap, renderString, writeLine and writeRect
// will return PCD8544_SUCCESS if they succeed and PCD8544_ERROR if they fail.
#define PCD8544_SUCCESS		1
#define PCD8544_ERROR		0


class PCD8544_SPI : public Print
{
public:
	PCD8544_SPI();	
	
	void begin(bool invert = false);
	void begin(bool invert, uint8_t vop, uint8_t tempCoef, uint8_t bias);
	void clear();
	uint8_t gotoXY(uint8_t x, uint8_t y);
	virtual size_t write(uint8_t uint8_t);
	uint8_t writeBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

private:

	//void init(void);
	void writeLcd(uint8_t dataOrCommand, uint8_t data);
	void writeLcd(uint8_t dataOrCommand, const uint8_t *data, uint16_t count);

	uint8_t m_Column;
	uint8_t m_Line;
	void advanceXY(uint8_t columns);
};

class PCD8544_SPI_FB : public Print
{
public:
	PCD8544_SPI_FB();	

	// Call a render method after any print/write methods are called.
	// For best perofrmance aggragate all writes before calling a render method.
	void renderAll();
	uint8_t renderString(uint8_t x, uint8_t y, uint16_t length);
	
	void setPixel(uint8_t x, uint8_t y, uint8_t value);
	
	// WriteLine currently only supports horizontal and vertical lines.
	uint8_t writeLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
	uint8_t writeRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill = false);
	
	void begin(bool invert = false);
	void begin(bool invert, uint8_t vop, uint8_t tempCoef, uint8_t bias);
	void clear(bool render = true);
	uint8_t gotoXY(uint8_t x, uint8_t y);
	virtual size_t write(uint8_t uint8_t);
	uint8_t writeBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

private:

	//void init(void);
	void writeLcd(uint8_t dataOrCommand, uint8_t data);
	void writeLcd(uint8_t dataOrCommand, const uint8_t *data, uint16_t count);

	inline void swap(uint8_t &a, uint8_t &b);
	uint16_t m_Position;
	uint8_t m_Buffer[BUF_LEN];
};

//This table contains the hex values that represent pixels
//for a font that is 5 pixels wide and 8 pixels high
static const PROGMEM uint8_t ASCII[][5] = {
	 { 0x00, 0x00, 0x00, 0x00, 0x00 } // 20   (space)
	,{ 0x00, 0x00, 0x5f, 0x00, 0x00 } // 21 !
	,{ 0x00, 0x07, 0x00, 0x07, 0x00 } // 22 "
	,{ 0x14, 0x7f, 0x14, 0x7f, 0x14 } // 23 #
	,{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 } // 24 $
	,{ 0x23, 0x13, 0x08, 0x64, 0x62 } // 25 %
	,{ 0x36, 0x49, 0x55, 0x22, 0x50 } // 26 &
	,{ 0x00, 0x05, 0x03, 0x00, 0x00 } // 27 '
	,{ 0x00, 0x1c, 0x22, 0x41, 0x00 } // 28 (
	,{ 0x00, 0x41, 0x22, 0x1c, 0x00 } // 29 )
	,{ 0x14, 0x08, 0x3e, 0x08, 0x14 } // 2a *
	,{ 0x08, 0x08, 0x3e, 0x08, 0x08 } // 2b +
	,{ 0x00, 0x50, 0x30, 0x00, 0x00 } // 2c ,
	,{ 0x08, 0x08, 0x08, 0x08, 0x08 } // 2d -
	,{ 0x00, 0x60, 0x60, 0x00, 0x00 } // 2e .
	,{ 0x20, 0x10, 0x08, 0x04, 0x02 } // 2f /
	,{ 0x3e, 0x51, 0x49, 0x45, 0x3e } // 30 0
	,{ 0x00, 0x42, 0x7f, 0x40, 0x00 } // 31 1
	,{ 0x42, 0x61, 0x51, 0x49, 0x46 } // 32 2
	,{ 0x21, 0x41, 0x45, 0x4b, 0x31 } // 33 3
	,{ 0x18, 0x14, 0x12, 0x7f, 0x10 } // 34 4
	,{ 0x27, 0x45, 0x45, 0x45, 0x39 } // 35 5
	,{ 0x3c, 0x4a, 0x49, 0x49, 0x30 } // 36 6
	,{ 0x01, 0x71, 0x09, 0x05, 0x03 } // 37 7
	,{ 0x36, 0x49, 0x49, 0x49, 0x36 } // 38 8
	,{ 0x06, 0x49, 0x49, 0x29, 0x1e } // 39 9
	,{ 0x00, 0x36, 0x36, 0x00, 0x00 } // 3a :
	,{ 0x00, 0x56, 0x36, 0x00, 0x00 } // 3b ;
	,{ 0x08, 0x14, 0x22, 0x41, 0x00 } // 3c <
	,{ 0x14, 0x14, 0x14, 0x14, 0x14 } // 3d =
	,{ 0x00, 0x41, 0x22, 0x14, 0x08 } // 3e >
	,{ 0x02, 0x01, 0x51, 0x09, 0x06 } // 3f ?
	,{ 0x32, 0x49, 0x79, 0x41, 0x3e } // 40 @
	,{ 0x7e, 0x11, 0x11, 0x11, 0x7e } // 41 A
	,{ 0x7f, 0x49, 0x49, 0x49, 0x36 } // 42 B
	,{ 0x3e, 0x41, 0x41, 0x41, 0x22 } // 43 C
	,{ 0x7f, 0x41, 0x41, 0x22, 0x1c } // 44 D
	,{ 0x7f, 0x49, 0x49, 0x49, 0x41 } // 45 E
	,{ 0x7f, 0x09, 0x09, 0x09, 0x01 } // 46 F
	,{ 0x3e, 0x41, 0x49, 0x49, 0x7a } // 47 G
	,{ 0x7f, 0x08, 0x08, 0x08, 0x7f } // 48 H
	,{ 0x00, 0x41, 0x7f, 0x41, 0x00 } // 49 I
	,{ 0x20, 0x40, 0x41, 0x3f, 0x01 } // 4a J
	,{ 0x7f, 0x08, 0x14, 0x22, 0x41 } // 4b K
	,{ 0x7f, 0x40, 0x40, 0x40, 0x40 } // 4c L
	,{ 0x7f, 0x02, 0x0c, 0x02, 0x7f } // 4d M
	,{ 0x7f, 0x04, 0x08, 0x10, 0x7f } // 4e N
	,{ 0x3e, 0x41, 0x41, 0x41, 0x3e } // 4f O
	,{ 0x7f, 0x09, 0x09, 0x09, 0x06 } // 50 P
	,{ 0x3e, 0x41, 0x51, 0x21, 0x5e } // 51 Q
	,{ 0x7f, 0x09, 0x19, 0x29, 0x46 } // 52 R
	,{ 0x46, 0x49, 0x49, 0x49, 0x31 } // 53 S
	,{ 0x01, 0x01, 0x7f, 0x01, 0x01 } // 54 T
	,{ 0x3f, 0x40, 0x40, 0x40, 0x3f } // 55 U
	,{ 0x1f, 0x20, 0x40, 0x20, 0x1f } // 56 V
	,{ 0x3f, 0x40, 0x38, 0x40, 0x3f } // 57 W
	,{ 0x63, 0x14, 0x08, 0x14, 0x63 } // 58 X
	,{ 0x07, 0x08, 0x70, 0x08, 0x07 } // 59 Y
	,{ 0x61, 0x51, 0x49, 0x45, 0x43 } // 5a Z
	,{ 0x00, 0x7f, 0x41, 0x41, 0x00 } // 5b [
	,{ 0x02, 0x04, 0x08, 0x10, 0x20 } // 5c backslash
	,{ 0x00, 0x41, 0x41, 0x7f, 0x00 } // 5d ]
	,{ 0x04, 0x02, 0x01, 0x02, 0x04 } // 5e ^
	,{ 0x40, 0x40, 0x40, 0x40, 0x40 } // 5f _
	,{ 0x00, 0x01, 0x02, 0x04, 0x00 } // 60 `
	,{ 0x20, 0x54, 0x54, 0x54, 0x78 } // 61 a
	,{ 0x7f, 0x48, 0x44, 0x44, 0x38 } // 62 b
	,{ 0x38, 0x44, 0x44, 0x44, 0x20 } // 63 c
	,{ 0x38, 0x44, 0x44, 0x48, 0x7f } // 64 d
	,{ 0x38, 0x54, 0x54, 0x54, 0x18 } // 65 e
	,{ 0x08, 0x7e, 0x09, 0x01, 0x02 } // 66 f
	,{ 0x0c, 0x52, 0x52, 0x52, 0x3e } // 67 g
	,{ 0x7f, 0x08, 0x04, 0x04, 0x78 } // 68 h
	,{ 0x00, 0x44, 0x7d, 0x40, 0x00 } // 69 i
	,{ 0x20, 0x40, 0x44, 0x3d, 0x00 } // 6a j 
	,{ 0x7f, 0x10, 0x28, 0x44, 0x00 } // 6b k
	,{ 0x00, 0x41, 0x7f, 0x40, 0x00 } // 6c l
	,{ 0x7c, 0x04, 0x18, 0x04, 0x78 } // 6d m
	,{ 0x7c, 0x08, 0x04, 0x04, 0x78 } // 6e n
	,{ 0x38, 0x44, 0x44, 0x44, 0x38 } // 6f o
	,{ 0x7c, 0x14, 0x14, 0x14, 0x08 } // 70 p
	,{ 0x08, 0x14, 0x14, 0x18, 0x7c } // 71 q
	,{ 0x7c, 0x08, 0x04, 0x04, 0x08 } // 72 r
	,{ 0x48, 0x54, 0x54, 0x54, 0x20 } // 73 s
	,{ 0x04, 0x3f, 0x44, 0x40, 0x20 } // 74 t
	,{ 0x3c, 0x40, 0x40, 0x20, 0x7c } // 75 u
	,{ 0x1c, 0x20, 0x40, 0x20, 0x1c } // 76 v
	,{ 0x3c, 0x40, 0x30, 0x40, 0x3c } // 77 w
	,{ 0x44, 0x28, 0x10, 0x28, 0x44 } // 78 x
	,{ 0x0c, 0x50, 0x50, 0x50, 0x3c } // 79 y
	,{ 0x44, 0x64, 0x54, 0x4c, 0x44 } // 7a z
	,{ 0x00, 0x08, 0x36, 0x41, 0x00 } // 7b {
	,{ 0x00, 0x00, 0x7f, 0x00, 0x00 } // 7c |
	,{ 0x00, 0x41, 0x36, 0x08, 0x00 } // 7d }
	,{ 0x10, 0x08, 0x08, 0x10, 0x08 } // 7e ~
	,{ 0x78, 0x46, 0x41, 0x46, 0x78 } // 7f DEL
};
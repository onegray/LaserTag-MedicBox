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


#include "PCD8544_SPI.h"
#include <SPI.h>

PCD8544_SPI::PCD8544_SPI()
{
}

void PCD8544_SPI::begin(bool invert)
{
	this->begin(invert, 0xB8, 0x04, 0x12);
}

void PCD8544_SPI::begin(bool invert, uint8_t vop, uint8_t tempCoef, uint8_t bias)
{
	PCD8544_PORT |= (PIN_DC | PIN_RESET | PIN_CE);
	PCD8544_DDR |= (PIN_DC | PIN_RESET | PIN_CE);
	SPI.begin();
#if PIN_CE != 0x04 // if D10 pin is not used for CE then disable it  -- SergeyN
	PCD8544_PORT &= ~0x04;
#endif        
	
	// LCD init section:
	
	uint8_t invertSetting = invert ? 0x0D : 0x0C;
	// Must reset LCD first!
	PCD8544_PORT &= ~PIN_RESET;
	PCD8544_PORT |= PIN_RESET;

	this->writeLcd(PCD8544_COMMAND, 0x21); //Tell LCD that extended commands follow
	this->writeLcd(PCD8544_COMMAND, vop); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
	this->writeLcd(PCD8544_COMMAND, tempCoef); //Set Temp coefficent
	this->writeLcd(PCD8544_COMMAND, bias); //LCD bias mode 1:48: Try 0x13 or 0x14. Mine works best with 1:65/1:65

	this->writeLcd(PCD8544_COMMAND, 0x20); //We must send 0x20 before modifying the display control mode
	this->writeLcd(PCD8544_COMMAND, invertSetting); //Set display control, normal mode. 0x0D for inverse

	this->clear();
}

size_t PCD8544_SPI::write(uint8_t data)
{
	// Non-ASCII characters are not supported.
	if (data < 0x20 || data > 0x7F) return 0;

	uint8_t buf[6];
	memcpy_P(buf, ASCII[data - 0x20], 5);
	buf[5] = 0x00;
	this->writeLcd(PCD8544_DATA, buf, 6);
	this->advanceXY(6);
	return 1;
}

void PCD8544_SPI::clear()
{
	for (uint16_t i = 0; i < BUF_LEN; i++)
		this->writeLcd(PCD8544_DATA, 0x00);
	//PCD8544_PORT = (PCD8544_PORT & ~PINS_CE_DC) | PCD8544_DATA;
	//for (uint16_t i = BUF_LEN; i > 0; i--) SPI.transfer(0x00);
	//PCD8544_PORT |= PIN_CE;
	this->gotoXY(0, 0);
}

uint8_t PCD8544_SPI::gotoXY(uint8_t x, uint8_t y) 
{	
	if (x >= PCD8544_X_PIXELS || y >= PCD8544_ROWS) return PCD8544_ERROR;
	//PCD8544_PORT = (PCD8544_PORT & ~PINS_CE_DC) | PCD8544_COMMAND;
	//SPI.transfer(0x80 | x);
	//SPI.transfer(0x40 | y);
	//PCD8544_PORT |= PIN_CE;
	this->writeLcd(PCD8544_COMMAND, 0x80 | x);  // Column.
	this->writeLcd(PCD8544_COMMAND, 0x40 | y);  // Row.
	this->m_Column = x;
	this->m_Line = y;
	return PCD8544_SUCCESS;
}

uint8_t PCD8544_SPI::writeBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	//if (x >= PCD8544_X_PIXELS || y >= PCD8544_ROWS) return;
	////height = (this->m_Line + height > PCD8544_Y_PIXELS / 8) ? ((PCD8544_Y_PIXELS / 8) - this->m_Line) : height;
	////width = (this->m_Column + width > PCD8544_X_PIXELS) ? (PCD8544_X_PIXELS - this->m_Column) : width;
	//this->gotoXY(x, y);
	//for (uint8_t y = 0; y < height; y++)
	//{
	//	for (uint8_t x = 0; x < width; x++)
	//		this->writeLcd(PCD8544_DATA, bitmap[x + (y * width)]);
	//	this->gotoXY(this->m_Column, this->m_Line + 1);
	//}

	//this->advanceXY(width);
	
	if (this->gotoXY(x, y) == PCD8544_ERROR) return PCD8544_ERROR;	
	const uint8_t *maxY = bitmap + height * width;	

	for (const uint8_t *y = bitmap; y < maxY; y += width)
	{
		//for (uint8_t x = 0; x < width; x++, y++)
		//	this->writeLcd(PCD8544_DATA, *y);
		
		this->writeLcd(PCD8544_DATA, y , width);
		this->gotoXY(this->m_Column, this->m_Line + 1);
	}

	this->advanceXY(width);
}

//void PCD8544_SPI::init(void) 
//{
//	// Must reset LCD first!
//	PCD8544_PORT &= ~PIN_RESET;
//	PCD8544_PORT |= PIN_RESET;
//
//	this->writeLcd(PCD8544_COMMAND, 0x21); //Tell LCD that extended commands follow
//	this->writeLcd(PCD8544_COMMAND, 0xB0); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
//	this->writeLcd(PCD8544_COMMAND, 0x04); //Set Temp coefficent
//	this->writeLcd(PCD8544_COMMAND, 0x12); //LCD bias mode 1:48: Try 0x13 or 0x14. Mine works best with 1:65/1:65
//
//	this->writeLcd(PCD8544_COMMAND, 0x20); //We must send 0x20 before modifying the display control mode
//	this->writeLcd(PCD8544_COMMAND, 0x0C); //Set display control, normal mode. 0x0D for inverse
//}

void PCD8544_SPI::advanceXY(uint8_t columns)
{
	this->m_Column += columns;
	if (this->m_Column >= PCD8544_X_PIXELS)
	{
		this->m_Column %= PCD8544_X_PIXELS;
		this->m_Line++;
		this->m_Line %= PCD8544_ROWS;
	}
}

void PCD8544_SPI::writeLcd(uint8_t dataOrCommand, const uint8_t *data, uint16_t count)
{
	PCD8544_PORT = (PCD8544_PORT & ~PINS_CE_DC) | dataOrCommand;
	//for (uint16_t i = 0; i < count; i++)
	//	SPI.transfer(data[i]);
    for (uint16_t i = count; i > 0; i--)
		SPI.transfer(data[count-i]);
	PCD8544_PORT |= PIN_CE;
}

void PCD8544_SPI::writeLcd(uint8_t dataOrCommand, uint8_t data)
{
	PCD8544_PORT = (PCD8544_PORT & ~PINS_CE_DC) | dataOrCommand;
	SPI.transfer(data);
	PCD8544_PORT |= PIN_CE;
}

//
//  mlt_core.cpp
//  MedicBox
//
//  Created by Sergey Nikitenko on 7/2/15.
//  Copyright (c) 2015, Sergey Nikitenko. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//


// Frequency calculations
// carrier frequency:   f = 56000 Hz
// wave period:         T = 1/f = 17.86 µs
// half cycle period:   T/2 = 8.93 µs

// assuming carrier_interval_delay + instructions_delay = T/2
#define CARRIER_INTERVAL	4		// adjusted experimentally


// MilesTag definitions
// carrier off space: 600 µs
// header pulse: 2400 µs
// bit0 pulse: 600 µs
// bit1 pulse: 1200 µs

#define MLT_HEADER_LENGTH 	2400
#define MLT_BIT0_LENGTH 	600
#define MLT_BIT1_LENGTH 	1200
#define MLT_SPACE_INTERVAL	600

// header cycle count: 2400 / 17.86 = 134.3
#define MLT_HEADER_CYCLES	128		// adjusted experimentally

// bit0 cycle count: 600 / 17.86 = 33.6
#define MLT_BIT0_CYCLES		32		// adjusted experimentally

// bit1 cycle count: 1200 / 17.86 = 67.2
#define MLT_BIT1_CYCLES		64		// adjusted experimentally


#define MLT_HEADER_MIN_LENGTH	(MLT_HEADER_LENGTH-500)
#define MLT_HEADER_MAX_LENGTH	(MLT_HEADER_LENGTH+300)
#define MLT_BIT0_MIN_LENGTH 	(MLT_BIT0_LENGTH-200)
#define MLT_BIT0_MAX_LENGTH 	(MLT_BIT0_LENGTH+200)
#define MLT_BIT1_MIN_LENGTH 	(MLT_BIT1_LENGTH-250)
#define MLT_BIT1_MAX_LENGTH 	(MLT_BIT1_LENGTH+250)

#define MLT_SHOT_DATA_LENGTH	14
#define MLT_CMD_DATA_LENGTH 	24
#define MLT_MAX_DATA_LENGTH 	24

#define PULSE_TIMEOUT	2000

#include <arduino.h>
#include "mlt_core.h"


void mltSetup() {
	pinMode(IR_EMITTER_PIN, OUTPUT);
	pinMode(IR_SENSOR_PIN, INPUT);
}

void sendPulse(int cycles) {
	for (int i=0; i<cycles; i++ ) {
		digitalWrite(IR_EMITTER_PIN, HIGH);
		delayMicroseconds(CARRIER_INTERVAL);
		digitalWrite(IR_EMITTER_PIN, LOW);
		delayMicroseconds(CARRIER_INTERVAL);
	}
}

void sendByte(char x, int bitCount) {
	for(int i=0; i<bitCount; i++) {
		if (x & 0x80) {
			sendPulse(MLT_BIT1_CYCLES);
		} else {
			sendPulse(MLT_BIT0_CYCLES);
		}
		x = x << 1;
		delayMicroseconds(MLT_SPACE_INTERVAL);
	}
}

void sendHeader() {
	sendPulse(MLT_HEADER_CYCLES);
	delayMicroseconds(MLT_SPACE_INTERVAL);
}

char scanBits(const char* bitBuf, char count) {
	char value = bitBuf[0];
	for (char i=1; i < count; i++) {
		value = value << 1;
		value = value | bitBuf[i];
	}
	return value;
}

void printBits(const char* bitBuf, char count) {
	Serial.print("Data: ");
	for(char i=0; i<count; i++) {
		if (i%8 == 0) {
			Serial.print(" ");
		}
		Serial.print((int)bitBuf[i]);
		Serial.print(" ");
	}
	Serial.println("");
}

struct mlt_command receiveCommand() {
	
	mlt_command cmd;
	cmd.command_type = MLT_CT_INVALID;
	
	if( digitalRead(IR_SENSOR_PIN) == LOW ) {
		
		unsigned long headerStartTime = micros();
		while ( digitalRead(IR_SENSOR_PIN) == LOW );
		unsigned long headerEndTime = micros();
		
		int headerLength = headerEndTime - headerStartTime;
		
		if ( headerLength > MLT_HEADER_MIN_LENGTH && headerLength < MLT_HEADER_MAX_LENGTH ) {
			
			char buf[MLT_MAX_DATA_LENGTH];
			char receivedPulseCount = 0;
			
			while (receivedPulseCount < MLT_MAX_DATA_LENGTH) {
				int pulseLength = 0;
				if( digitalRead(IR_SENSOR_PIN) == HIGH ) {
					pulseLength = pulseIn(IR_SENSOR_PIN, LOW, PULSE_TIMEOUT);
				}
				
				if(pulseLength > MLT_BIT0_MIN_LENGTH && pulseLength < MLT_BIT0_MAX_LENGTH ) {
					buf[receivedPulseCount] = 0;
					receivedPulseCount++;
				} else if(pulseLength > MLT_BIT1_MIN_LENGTH && pulseLength < MLT_BIT1_MAX_LENGTH ) {
					buf[receivedPulseCount] = 1;
					receivedPulseCount++;
				} else if (pulseLength > MLT_HEADER_MIN_LENGTH && pulseLength < MLT_HEADER_MAX_LENGTH) {
					receivedPulseCount = 0;
					continue;
				} else {
					break;
				}
			}
			
			if ( receivedPulseCount==MLT_SHOT_DATA_LENGTH && buf[0]==0 ) {
				cmd.command_type = MLT_CT_SHOT;
				cmd.shot_data.player_id = scanBits(buf+1, 7);
				cmd.shot_data.team_color = (mlt_team_color)scanBits(buf+8, 2);
				cmd.shot_data.damage = (mlt_shot_damage)scanBits(buf+10, 4);
			} else if ( receivedPulseCount==MLT_CMD_DATA_LENGTH && buf[0]==1 ) {
				char byte0 = scanBits(buf, 8);
				char byte1 = scanBits(buf+8, 8);
				char byte2 = scanBits(buf+16, 8);
				
				if(byte0 == (char)0x83 && byte2 == (char)0xE8) {
					cmd.command_type = MLT_CT_SYSTEM;
					cmd.sys_cmd = (mlt_system_command)byte1;
				}
			}
			
			//Serial.print("Header: ");
			//Serial.println(headerLength);
			//printBits(buf, receivedPulseCount);
		}
	}
	
	return cmd;
}

void sendSystemCommand(mlt_system_command cmd) {
	sendHeader();
	sendByte(0x83, 8);
	sendByte(cmd, 8);
	sendByte(0xE8, 8);
}

void sendShotCommand(char player_id, mlt_team_color color, mlt_shot_damage damage) {
	sendHeader();
	sendByte((player_id & 0x7F), 8);
	sendByte((color << 6), 2);
	sendByte((damage << 4), 4);
}

void printCommand(mlt_command cmd) {
	if (cmd.command_type == MLT_CT_SHOT) {
		Serial.println("MLT_CT_SHOT");
		Serial.print(" player: ");
		Serial.println((int)cmd.shot_data.player_id);
		Serial.print(" color: ");
		Serial.println((int)cmd.shot_data.team_color);
		Serial.print(" damage: ");
		Serial.println((int)cmd.shot_data.damage);
		Serial.println("\n\n");
	} else if (cmd.command_type == MLT_CT_SYSTEM) {
		Serial.println("MLT_CT_SYSTEM");
		Serial.print("cmd = ");
		Serial.print((int)cmd.sys_cmd);
		Serial.println("\n\n");
	}
}

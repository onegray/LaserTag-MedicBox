//
//  MedicBox.ino
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

#include <Arduino.h>
#include <avr/sleep.h>
#include <SPI.h>

#include "mlt_core.h"
#include "SmartMedic.h"
#include "Device.h"

#define	BTN_PIN   	3
#define SOUND_PIN  	5

static Device* device = NULL;
static SmartMedic* medic = NULL;

void setup() {

	mltSetup();

	pinMode(BTN_PIN, INPUT);
	digitalWrite(BTN_PIN, HIGH);

	device = new Device();
	//medic = new SmartMedic(device);

	Serial.begin(9600);
	Serial.println("Ready...");
}

void loop() {
	
	mlt_command cmd = receiveCommand();
	bool btnPressed = digitalRead(BTN_PIN) == LOW;
	
	if ( medic!=NULL ) {

		if (medic->processCommand(&cmd)) {
		
			
		} else if (btnPressed) {
			sendSystemCommand(MLT_SC_NEW_GAME);
			delay(100);
		}
		
	}

	if(device!=NULL) {
		device->showStatusText("Shot to start.");
	}
	
	bool testMode = true;
	if(testMode) {

		if(cmd.command_type == MLT_CT_SHOT) {
			delay(300);
			testStunTime();
			delay(300);
			sendSystemCommand(MLT_SC_NEW_GAME);
			delay(1000);
		}

	}

	//tone(SOUND_PIN, 1000, 150);
	delay(10);
	Sleep();
}

void Sleep() {
	sleep_enable();
	attachInterrupt(0, onWakeUp, LOW);
	attachInterrupt(1, onWakeUp, LOW);
	set_sleep_mode(SLEEP_MODE_STANDBY);
	
	sleep_cpu();
	
	sleep_disable();
	detachInterrupt(0);
	detachInterrupt(1);
}

void onWakeUp() {
}


void testStunTime() {
	
	if(device!=NULL) {
		device->showStatusText("Waiting for shot...");
	}

	sendShotCommand(0, MLT_ST_YELLOW, MLT_SHOT_DAMAGE_1);
	
	unsigned long startTime = micros();
	unsigned long endTime = startTime;
	
	mlt_command cmd = receiveCommand();
	while (cmd.command_type != MLT_CT_SHOT) {
		endTime = micros();
		cmd = receiveCommand();
	}

	long stunTime = endTime - startTime;
	
	if(device!=NULL) {
		device->showTimeInterval(stunTime/1000, "Stun time:");
	} else {
		Serial.print("Stun time: ");
		Serial.print(stunTime/1000000);
		Serial.print(".");
		Serial.println((stunTime%1000000)/1000);
	}
}


void testImmortalTime() {

	sendSystemCommand(MLT_SC_NEW_GAME);
	delay(3000);

	for(int i=0; i<4; i++) {
		sendShotCommand(0, MLT_ST_YELLOW, MLT_SHOT_DAMAGE_1);
		delay(1300);
	}
	
}








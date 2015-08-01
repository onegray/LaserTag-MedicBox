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
#include "MedicBox.h"
#include "ModeMenu.h"
#include "Device.h"

#define	BTN_PIN   	3

#define IsValidCmd(cmd)         (cmd.command_type != MLT_CT_INVALID)
#define IsChangeModeCmd(cmd)    (cmd.command_type == MLT_CT_SYSTEM && cmd.sys_cmd == MLT_SC_CHANGE_COLOR)
#define IsResetCmd(cmd)         (cmd.command_type == MLT_CT_SYSTEM && cmd.sys_cmd == MLT_SC_NEW_GAME)
#define IsEscCmd(cmd)           (cmd.command_type == MLT_CT_SYSTEM && cmd.sys_cmd == MLT_SC_ADMIN_KILL)


static Device* device = NULL;
static ModeMenu* menu = NULL;
static MedicBox* medic = NULL;


void setup() {

	pinMode(BTN_PIN, INPUT);
	digitalWrite(BTN_PIN, HIGH);

	mltSetup();

	device = new Device();
	//medic = new MedicBox(device);

	Serial.begin(9600);
	delay(50);
	
	device->showDeviceReady();
	Serial.println("Ready...");
}

void loop() {
	
	mlt_command cmd = receiveCommand();
	bool btnPressed = (digitalRead(BTN_PIN) == LOW);
	
	if ( IsChangeModeCmd(cmd) ) {
		if(menu == NULL) {
			menu = new ModeMenu(device);
		} else {
			menu->changeMode();
		}
		goto sleep;
	}
	
	if (menu != NULL) {
		
		if ( IsResetCmd(cmd) ) {
			if(medic != NULL) {
				delete medic;
			}
			medic = menu->instantiateMedicBox();
			if (medic != NULL) {
				medic->reset();
			}
			delete menu;
			menu = NULL;
		} else if ( IsEscCmd(cmd) ) {

		} else if (btnPressed) {
			menu->changeModeParam();
		}
		goto sleep;
	}
	
	if (medic != NULL) {

		if ( IsResetCmd(cmd) ) {
			medic->reset();
		} else if ( IsValidCmd(cmd) ) {
			medic->processCommand(&cmd);
		} else if (btnPressed) {
			medic->processButton();
		}
		goto sleep;
	}


sleep:
	delay(50);
	Sleep();
	
}

void Sleep() {
	
//	MCUSR &= ~(1<<WDRF); // Clear the reset flag
//	WDTCSR |= (1<<WDCE) | (1<<WDE);
//	WDTCSR = 1<<WDP0 | 1<<WDP3; // timeout prescaler for 8.0 seconds
//	WDTCSR |= _BV(WDIE); // Enable the WD interrupt
	
	sleep_enable();
	attachInterrupt(0, onWakeUp, LOW);
	attachInterrupt(1, onWakeUp, LOW);
	set_sleep_mode(SLEEP_MODE_STANDBY);
	
	sleep_cpu();
	
	sleep_disable();
	detachInterrupt(0);
	detachInterrupt(1);
}


//ISR(WDT_vect) {
//	//Serial.println("WDT event");
//}

void onWakeUp() {
}






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
#include <EEPROM.h>

#include "Circuit.h"
#include "mlt_core.h"
#include "MedicBox.h"
#include "ModeMenu.h"
#include "Device.h"
#include "ConfigurationProfile.h"


#define IsValidCmd(cmd)         (cmd.command_type != MLT_CT_INVALID)
#define IsChangeModeCmd(cmd)    (cmd.command_type == MLT_CT_SYSTEM && cmd.sys_cmd == MLT_SC_CHANGE_COLOR)
#define IsResetCmd(cmd)         (cmd.command_type == MLT_CT_SYSTEM && cmd.sys_cmd == MLT_SC_NEW_GAME)
#define IsEscCmd(cmd)           (cmd.command_type == MLT_CT_SYSTEM && cmd.sys_cmd == MLT_SC_ADMIN_KILL)


static Device* device = NULL;
static ModeMenu* menu = NULL;
static MedicBox* medic = NULL;
static ConfigurationProfile* config = NULL;

void setup() {

	config = new ConfigurationProfile();
	device = new Device();

	delay(100);
	
	if(config->getBacklightMode()) {
		device->turnBacklight(true);
		delay(50);
	}	

	//medic = ModeMenu::instantiateMedicBox(device, config);
	medic = new DominationTube(device, 60);
	
	if (medic != NULL) {
		medic->reset();
	} else {
		device->showDeviceReady();
	}
	
	//Serial.begin(9600);
	//Serial.println("Ready...");
	delay(10);
}

void loop() {
	
	mlt_command cmd = device->receiveCommand();
	bool btnPressed = 0; //device->isButtonPressed();
	
	if ( IsChangeModeCmd(cmd) ) {
		if(menu == NULL) {
			menu = new ModeMenu(device, config);
		} else {
			menu->changeMode();
		}
		goto sleep;
	}
	
	if (menu != NULL) {
		
		if ( IsResetCmd(cmd) ) {
			
			menu->saveConfig();
			delete menu;
			menu = NULL;
			
			if(medic != NULL) {
				delete medic;
			}
			medic = ModeMenu::instantiateMedicBox(device, config);
			if (medic != NULL) {
				medic->reset();
			}
		} else if ( IsEscCmd(cmd) ) {

		} else if (btnPressed) {
			menu->changeBacklight();
			delay(300);
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
		} else {
			medic->updateTime();
		}
		goto sleep;
	}


sleep:
	if(device->canSleep()) {
		Sleep();
	}
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






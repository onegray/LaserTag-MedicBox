//
//  MBTagTest.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 9/29/17.
//  Copyright (c) 2015-2017, Sergey Nikitenko. All rights reserved.
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



#ifndef MB_TAGTEST_H
#define MB_TAGTEST_H

#include "MBInterface.h"
#include "Device.h"

class StunTimeTest : public MedicBox
{
public:
	
	StunTimeTest(Device* aDevice)
	: MedicBox(aDevice) {
	}
	
	virtual void reset() {
		device->showMedicBoxReady();
		device->showStatusText("Shot to start");
	}
	
	virtual void processButton() {
		device->sendNewGameCommand();
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			startTest();
		}
	}
	
	void startTest() {
		device->showStatusText("Waiting for shot...");
		delay(10);
		
		device->sendShotCommand();
		
		unsigned long startTime = micros();
		unsigned long endTime = startTime;
		
		mlt_command cmd = device->receiveCommand();
		while (cmd.command_type != MLT_CT_SHOT) {
			endTime = micros();
			cmd = device->receiveCommand();
		}
		
		long stunTime = endTime - startTime;
		
		device->showTimeInterval(stunTime/1000, "Stun time:");
		delay(500);
	}
	
};


class InvulnerabilityTimeTest : public MedicBox
{
public:
	
	InvulnerabilityTimeTest(Device* aDevice, ConfigurationProfile* aConfig)
	: MedicBox(aDevice, aConfig) {
		invulnerabilityTime = config->getTestInvulnerabilityTime();
	}
	
	virtual void reset() {
		device->showMedicBoxReady();
		device->showStatusText("Shot to start");
	}
	
	virtual void processButton() {
		device->sendNewGameCommand();
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			device->showStatusText("Shooting 4 times ...");
			startTest();
		}
	}
	
	void startTest() {
		delay(300);
		for(int i=0; i<4; i++) {
			device->sendShotCommand();
			delay(invulnerabilityTime);
		}
		device->showStatusText("Done.                ");
		delay(1000);
		device->showStatusText("Shot to start");
	}
	
protected:
	int invulnerabilityTime;
};


static int timeValues[] = {1.29*1000, 1.3*1000, 1.31*1000, 1.32*1000, 1.49*1000, 1.5*1000, 1.51*1000, 1.52*1000};

class InvulnerabilityTestSubmenu : public MedicBoxSubmenu
{
protected:
	int index;
	
public:
	InvulnerabilityTestSubmenu(Device* aDevice, ConfigurationProfile* aConfig)
	: MedicBoxSubmenu(aDevice, aConfig) {
		index = -1;
	}

	virtual void reset() {
		device->showMedicBoxReady();
		device->showStatusText(" Press button to set timeout");
	}

	virtual void processButton() {
		index = (index+1) % (sizeof(timeValues)/sizeof(timeValues[0]));
		updateDisplay();
		delay(300);
	}

	virtual void saveConfig() {
		if (index >= 0) {
			int value = timeValues[index];
			config->saveTestInvulnerabilityTime(value);
		}
	}

	void updateDisplay() {
		device->display.clearScreen();
		device->display.displayText(5, 0, "time:");
		if (index>=0) {
			int value = timeValues[index];
			device->display.displayFloating(value, 1);
		}
	}
	
};



#endif /* MB_TAGTEST_H */

//
//  SmartMedic.h
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


#ifndef SMART_MEDIC_H
#define SMART_MEDIC_H

#import "mlt_core.h"
#import "Device.h"


class MedicBox
{
public:
	MedicBox(Device* aDevice) {
		device = aDevice;
	}
	
	virtual void reset() {
		device->showMedicBoxReady();
		device->showStatusText("   - /--/ -   ");
	}

	virtual void processButton() {
		sendSystemCommand(MLT_SC_NEW_GAME);
		device->showRespawn();
		delay(220);
	}

	virtual void processCommand(mlt_command* cmd) {
	}
	
protected:
	Device* device;
};


class StatisticMedicBox : public MedicBox
{
public:
	
	StatisticMedicBox(Device* aDevice)
	: MedicBox(aDevice) {
	}
	
	virtual void reset() {
		respawnCount = 0;
		device->showMedicBoxReady();
		device->showRespawnNumber(respawnCount);
		device->preventSleep(1550);
	}
	
	virtual void processButton() {
		static unsigned long lastRespawnTime = 0;
		unsigned long currentTime = millis();
		if(currentTime > lastRespawnTime + 300) {
			sendSystemCommand(MLT_SC_NEW_GAME);
			if(currentTime > lastRespawnTime + 1500) {
				respawnCount++;
				device->showRespawnNumber(respawnCount);
			}
			device->showRespawn();
			lastRespawnTime = currentTime;
			device->preventSleep(1550);
		}
	}
	
	virtual void processCommand(mlt_command* cmd) {
	}
	
protected:
	int respawnCount;
};


class AliveMedicBox : public MedicBox
{
public:
	
	AliveMedicBox(Device* aDevice, int healthParam)
	: MedicBox(aDevice) {
		health = maxHealth = healthParam;
	}

	virtual void reset() {
		health = maxHealth;
		device->showMedicBoxReady();
		device->showHealthNumber(health);
	}
	
	virtual void processButton() {
		if(health > 0) {
			sendSystemCommand(MLT_SC_NEW_GAME);
			device->showRespawn();
			delay(250);
		}
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			if (health > 0) {
				health--;

				device->showHit();
				delay(300);
				
				if(health > 0) {
					device->showHealthNumber(health);
				} else {
					device->showGameOver();
				}
			}
		}
	}
	
protected:
	int health;
	int maxHealth;
};


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
		sendSystemCommand(MLT_SC_NEW_GAME);
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			startTest();
		}
	}

	void startTest() {
		device->showStatusText("Waiting for shot...");
		delay(10);
		
		sendShotCommand(0, MLT_ST_YELLOW, MLT_SHOT_DAMAGE_1);
		
		unsigned long startTime = micros();
		unsigned long endTime = startTime;
		
		mlt_command cmd = receiveCommand();
		while (cmd.command_type != MLT_CT_SHOT) {
			endTime = micros();
			cmd = receiveCommand();
		}
		
		long stunTime = endTime - startTime;
		
		device->showTimeInterval(stunTime/1000, "Stun time:");
		delay(500);
	}
	
};


class InvulnerabilityTimeTest : public MedicBox
{
public:
	
	InvulnerabilityTimeTest(Device* aDevice, int invulnerabilityTime)
	: MedicBox(aDevice) {
		this->invulnerabilityTime = invulnerabilityTime;
	}
	
	virtual void reset() {
		device->showMedicBoxReady();
		device->showStatusText("Shot to start");
	}
	
	virtual void processButton() {
		sendSystemCommand(MLT_SC_NEW_GAME);
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
			sendShotCommand(0, MLT_ST_YELLOW, MLT_SHOT_DAMAGE_1);
			delay(invulnerabilityTime);
		}
		device->showStatusText("Done.                ");
		delay(1000);
		device->showStatusText("Shot to start");
	}
	
protected:
	int invulnerabilityTime;
};


#endif

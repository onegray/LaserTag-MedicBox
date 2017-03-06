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

#include "Device.h"

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
		device->sendNewGameCommand();
		device->showRespawn();
		delay(220);
	}

	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			device->showHit();
			delay(100);
			device->showStatusText("   - /--/ -   ");
		}
	}
	
	virtual void updateTime() {
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
		static unsigned long lastRegisteredTime = 0;
		unsigned long currentTime = millis();

		if(currentTime > lastRespawnTime + 300) {
			lastRespawnTime = currentTime;
			device->sendNewGameCommand();
			device->showRespawn();
			device->preventSleep(350);
			
			if(currentTime > lastRegisteredTime + 1500) {
				lastRegisteredTime = currentTime;
				respawnCount++;
				device->showRespawnNumber(respawnCount);
				device->preventSleep(1550);
			}
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


class TempMedicBox : public MedicBox
{
public:
	
	TempMedicBox(Device* aDevice, int aliveTime, int stunTime)
	: MedicBox(aDevice) {
		this->aliveTime = aliveTime;
		this->stunTime = stunTime < 10*60 ? stunTime : 10*60-1;
	}
	
	virtual void reset() {
		alive = false;
		startTime = millis() / 1000;
		elapsedTime = 0;
		device->showMedicBoxReady();
		updateTime();
		device->preventSleep(2000);
	}
	
	virtual void processButton() {
		if(alive) {
			sendSystemCommand(MLT_SC_NEW_GAME);
			delay(300);
		}
		updateTime();
	}
	
	virtual void processCommand(mlt_command* cmd) {
	}

	virtual void updateTime() {
		unsigned long currentTime = millis() / 1000;
		int delta = currentTime - startTime;
		if( elapsedTime != delta ) {
			elapsedTime = delta;
			int timeLeft = (alive ? aliveTime : stunTime) - elapsedTime;
			if(timeLeft < 0) {
				alive = !alive;
				startTime = currentTime;
				timeLeft = (alive ? aliveTime : stunTime);
				device->playWarningBeep();
			}
			if(alive) {
				if(timeLeft < 60) {
					device->showTimerNumber(timeLeft, " Ready! ");
					device->playConfirmBeep();
				} else {
					device->showStatusText(" Ready! ");
				}
			} else {
				device->showTimeInterval(((long)timeLeft*100/60)*10, " Waiting... ");
			}
			device->preventSleep(2000);
		}
	}

protected:
	int aliveTime;
	int stunTime;
	bool alive;
	int elapsedTime;
	unsigned long startTime;
};


class TempAliveMedicBox : public TempMedicBox
{
public:
	TempAliveMedicBox(Device* aDevice, int stunTime)
	: TempMedicBox(aDevice, 9999, stunTime) {
	}

	virtual void reset() {
		TempMedicBox::reset();
		alive = true;
		updateTime();
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(alive && cmd->command_type == MLT_CT_SHOT) {
			alive = false;
			startTime = millis() / 1000;
			device->showHit();
			updateTime();
		}
	}
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
	
	InvulnerabilityTimeTest(Device* aDevice, int invulnerabilityTime)
	: MedicBox(aDevice) {
		this->invulnerabilityTime = invulnerabilityTime;
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


class DominationTube : public MedicBox
{
public:
	DominationTube(Device* aDevice, int winTime)
	: MedicBox(aDevice) {
		this->winTime = winTime;
	}

	virtual void reset() {
		isGameOver = false;
		lastSwitchTime = millis() / 1000;
		savedTimeRed = 0;
		savedTimeBlue = 0;
		currentColor = (mlt_team_color)-1;
		device->setRed(0);
		device->showMedicBoxReady();
		device->preventSleep(1000);
	}
	
	virtual void processButton() {
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			if(!isGameOver && cmd->shot_data.team_color != currentColor) {

				int currentTime = millis() / 1000;
				int delta = currentTime - lastSwitchTime;
				int savedTime = currentColor == MLT_ST_RED ? savedTimeRed : savedTimeBlue;
				lastSwitchTime = currentTime;
				
				if (currentColor == MLT_ST_RED) {
					savedTimeRed += delta;
				} else if (currentColor == MLT_ST_BLUE) {
					savedTimeBlue += delta;
				}
				
				currentColor = cmd->shot_data.team_color;
				
				if (currentColor == MLT_ST_RED) {
					device->setRed(127);
				} else {
					device->setBlue(127);
				}
				
				updateDisplay();
				device->playConfirmBeep();
			}
		}
	}
	
	virtual void updateTime() {
		static int oldTime = 0;
		int currentTime = millis() / 1000;

		if (oldTime != currentTime) {
			oldTime = currentTime;

			int delta = currentTime - lastSwitchTime;
			int savedTime = currentColor == MLT_ST_RED ? savedTimeRed : savedTimeBlue;
			
			if ( savedTime + delta < winTime ) {

				updateDisplay();
				
			} else {
				
				if (!isGameOver) {
					isGameOver = true;
					device->playWarningBeep();
				}
				
				int v = (currentTime % 2 == 0) ? 0 : 127;
				if (currentColor == MLT_ST_RED) {
					device->setRed(v);
				} else {
					device->setBlue(v);
				}
			}
			device->preventSleep(1000);
		}
	}

	void updateDisplay() {
		int currentTime = millis() / 1000;
		int delta = currentTime - lastSwitchTime;
		int savedTime = currentColor == MLT_ST_RED ? savedTimeRed : savedTimeBlue;
		
		int timeValue = ((long)(savedTime + delta)*100/60)*10;
		const char* text = currentColor == MLT_ST_RED ? "  Red  " : "  Blue  ";
		device->showTimeInterval(timeValue, text);
	}
	
protected:
	int winTime;
	int lastSwitchTime;
	int savedTimeRed;
	int savedTimeBlue;
	bool isGameOver;
	mlt_team_color currentColor;
};


#endif

//
//  MBAnomalyPoint.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 9/23/17.
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

#ifndef ANOMALY_POINT_MEDIC_H
#define ANOMALY_POINT_MEDIC_H

#include "MBInterface.h"
#include "Device.h"

class AnomalyPointMedicBox : public MedicBox
{
public:
	
	AnomalyPointMedicBox(Device* aDevice, ConfigurationProfile* aConfig)
	: MedicBox(aDevice, aConfig) {
		stunTime = config->getAnomalyPointStunTime();
		shotInterval = config->getAnomalyPointShotInterval();
		cmd = config->getAnomalyPointCommand();
		maxHealth = 8;
	}

	virtual void reset() {
		startTime = millis() / 1000;
		elapsedTime = 0;
		health = maxHealth;
		device->showMedicBoxReady();
		updateTime();
		device->preventSleep(2000);
	}
	
	virtual void processButton() {
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			if (health > 0) {
				health--;
				device->showHit();
				delay(1500);
				updateDisplay();
			}
		}
	}
	
	virtual void updateTime() {
		unsigned long currentTime = millis() / 1000;
		int delta = currentTime - startTime;
		if( elapsedTime != delta ) {
			elapsedTime = delta;
			int timeLeft = (health > 0 ? shotInterval : stunTime) - elapsedTime;
			if(timeLeft < 0) {
				if (health > 0) {
					shot();
				} else {
					health = maxHealth;
				}
				device->playWarningBeep();
				startTime = currentTime;
			}
			updateDisplay();
			device->preventSleep(2000);
		}
	}

	void shot() {
		if(cmd.command_type == MLT_CT_SYSTEM) {
			sendSystemCommand(cmd.sys_cmd);
		} else if(cmd.command_type == MLT_CT_SHOT) {
			sendShotCommand(cmd.shot_data.player_id, cmd.shot_data.team_color, cmd.shot_data.damage);
		}
	}
	
	void updateDisplay() {
		if(health > 0) {
			device->showHealthNumber(health);
		} else {
			int timeLeft = stunTime - elapsedTime;
			device->showTimeInterval(((long)timeLeft*100/60)*10, " Waiting... ");
		}
	}
	
protected:
	int stunTime;
	int shotInterval;
	int elapsedTime;
	unsigned long startTime;
	int health;
	int maxHealth;
	mlt_command cmd;
};


static const int intervals[][2] =  {{5,60}, {10,60}, {5,120}, {10,120}, {15,120}};

class AnomalyPointSubmenu : public MedicBoxSubmenu
{
protected:
	mlt_command mltCmd;
	int intervalIndex;

public:
	
	AnomalyPointSubmenu(Device* aDevice, ConfigurationProfile* aConfig)
	: MedicBoxSubmenu(aDevice, aConfig) {
		intervalIndex = -1;
		mltCmd.command_type = MLT_CT_INVALID;
	}

	virtual void reset() {
		device->showMedicBoxReady();
		device->showStatusText(" Press button to set timeout or make shot to record");
	}

	virtual void processButton() {
		intervalIndex = (intervalIndex+1) % (sizeof(intervals)/sizeof(intervals[0]));
		updateDisplay();
		delay(300);
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type != MLT_CT_INVALID) {
			mltCmd = *cmd;
			updateDisplay();
		}
	}
	
	virtual void saveConfig() {
		if (intervalIndex >= 0) {
			int shotInterval = intervals[intervalIndex][0];
			int stunTime = intervals[intervalIndex][1];
			config->saveAnomalyPointShotInterval(shotInterval);
			config->saveAnomalyPointStunTime(stunTime);
		}
		if(mltCmd.command_type != MLT_CT_INVALID) {
			config->saveAnomalyPointCommand(&mltCmd);
		}
	}

	void updateDisplay() {
		device->display.clearScreen();
		device->display.displayText(5, 0, "Params:");
		
		if (intervalIndex >=0) {
			int shotInterval = intervals[intervalIndex][0];
			int stunTime = intervals[intervalIndex][1];
			char shotBuf[12] = "shot: ";
			itoa(shotInterval, shotBuf+6, 10);
			device->display.displayText(0, 1, shotBuf);
			char stunBuf[12] = "stun: ";
			itoa(stunTime, stunBuf+6, 10);
			device->display.displayText(0, 2, stunBuf);
		}
		
		if(mltCmd.command_type == MLT_CT_SYSTEM) {
			char cmdText[12] = "cmd: ";
			itoa(mltCmd.sys_cmd, cmdText+5, 10);
			device->display.displayText(0, 3, cmdText);
		}
		if(mltCmd.command_type == MLT_CT_SHOT) {
			char shotText[16] = "clr_X, dmg_X";
			shotText[4] = 0x30 + mltCmd.shot_data.team_color;
			shotText[11] = 0x30 + mltCmd.shot_data.damage;
			device->display.displayText(0, 3, shotText);
		}
	}

};


#endif // ANOMALY_POINT_MEDIC_H

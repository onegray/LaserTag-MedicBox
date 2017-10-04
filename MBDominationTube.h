//
//  MBAnomalyPoint.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 9/30/17.
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


#ifndef MB_DOMINATION_TUBE_H
#define MB_DOMINATION_TUBE_H

#include "MBInterface.h"
#include "Device.h"

#define CLAMP_2_9(x) (2+(((x)-2)%8))

// Optimized divisions
#define DIV1000(x)	((x)*131/131072)
#define DIV500(x)	((x)*131/65536)
#define DIV250(x)	((x)*262/65536)
#define DIV100(x)	((x)*41/4096)

static const char* colorTitles[4] = {"Red:", "Blue:", "Yellow:", "Green:"};

class DominationTube : public MedicBox
{
public:
	DominationTube(Device* aDevice, ConfigurationProfile* aConfig)
	: MedicBox(aDevice, aConfig) {
		uint8_t value = config->getDominationTubeParam();
		this->winTime = CLAMP_2_9(value) * 4;
	}
	
	virtual void reset() {
		isGameOver = false;
		lastSwitchTime = DIV250(millis());
		for (uint8_t i=0; i<4; i++) {
			savedTimes[i] = 0;
		}
		currentTotalTime = 0;
		currentColor = (mlt_team_color)-1;
		previosColor = (mlt_team_color)-1;
		device->setWhite(true);
		device->showMedicBoxReady();
		device->showStatusText("Shot to start");
		device->preventSleep(10000);
	}
	
	virtual void processButton() {
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			if(!isGameOver && cmd->shot_data.team_color != currentColor) {
				unsigned currentTime = DIV250(millis());
				unsigned delta = currentTime - lastSwitchTime;
				lastSwitchTime = currentTime;

				savedTimes[currentColor] += delta;
				
				previosColor = currentColor;
				currentColor = cmd->shot_data.team_color;
				currentTotalTime = savedTimes[currentColor];
				
				updateDisplay();
				device->setColor(currentColor);
				device->playConfirmBeep();
			}
		}
	}
	
	virtual void updateTime() {
		static unsigned oldTime = 0;
		unsigned currentTime = DIV250(millis());
		if ( oldTime != currentTime && (int)currentColor != -1 ) {
			oldTime = currentTime;
			
			unsigned delta = currentTime - lastSwitchTime;
			currentTotalTime = savedTimes[currentColor] + delta;
			
			if ( currentTotalTime < winTime ) {
				updateDisplay();
			} else {
				if (!isGameOver) {
					isGameOver = true;
					device->playGameOver();
					device->showGameOver();
				}
				bool on = (currentTime % 4 < 2);
				device->setColor( on ? currentColor : (mlt_team_color)-1 );
			}
			device->preventSleep(10000);
		}
	}
	
	void updateDisplay() {
		unsigned displayTime = currentTotalTime / 4;
		static unsigned oldDisplayTime = 0;
		if (displayTime != oldDisplayTime) {
			oldDisplayTime = displayTime;

			device->display.clearScreen();

			device->display.displayText(3, 0, colorTitles[currentColor]);
			device->display.displayTime(displayTime, 1);

			if ( (int)previosColor != -1 ) {
				const char prevTimeText[5];
				itoa(prevTimeText, savedTimes[previosColor], 10);
				device->display.displayText(0, 5, colorTitles[previosColor]);
				device->display.displayText(6, 5, prevTimeText);
			}
		}
	}
	
protected:
	bool isGameOver;
	unsigned winTime;
	unsigned savedTimes[4];
	unsigned lastSwitchTime;
	unsigned currentTotalTime;
	mlt_team_color currentColor;
	mlt_team_color previosColor;
};



class DominationTubeSubmenu : public MedicBoxSubmenu
{
protected:
	uint8_t value;
	
public:
	DominationTubeSubmenu(Device* aDevice, ConfigurationProfile* aConfig)
	: MedicBoxSubmenu(aDevice, aConfig) {
		value = config->getDominationTubeParam();
	}
	
	virtual void reset() {
		device->display.clearScreen();
		updateDisplay();
		device->speaker.playBeep4();
	}
	
	virtual void processButton() {
		value = CLAMP_2_9(value+1);
		updateDisplay();
		delay(300);
	}
	
	virtual void saveConfig() {
		config->saveDominationTubeParam(value);
	}
	
	void updateDisplay() {
		char text[] = "time: X min";
		text[6] = 0x30 + CLAMP_2_9(value);
		device->display.displayText(0, 0, text);
	}
};



struct TDMParams {
	uint8_t startDelay;
	uint8_t matchDelay;
	uint8_t matchDuration;
};

static const TDMParams tdmParams[] = {
	{5, 5, 30},
	{5, 5, 40},
	{5, 7, 30},
	{5, 7, 40},
	{5, 9, 40},
	{35, 5, 30},
	{35, 5, 40},
	{35, 7, 30},
	{35, 7, 40},
	{35, 9, 40},
	{90, 7, 40},
};


class DominationTubeTDM : public MedicBox
{
public:
	DominationTubeTDM(Device* aDevice, ConfigurationProfile* aConfig)
	: MedicBox(aDevice, aConfig) {
		uint8_t index = config->getTdmParam();
		if (index > sizeof(tdmParams)/sizeof(tdmParams[0])) {
			index = 0;
		}
		this->startDelayTime = tdmParams[index].startDelay * 4;
		this->matchDelayTime = tdmParams[index].matchDelay * 60 * 4;
		this->winTime = tdmParams[index].matchDuration * 4;
	}
	
	virtual void reset() {
		isGameStarted = false;
		isMatchStarted = false;
		isGameOver = false;
		restartTime = DIV250(millis());
		lastSwitchTime = 0;
		currentColor = (mlt_team_color)-1;
		previosColor = (mlt_team_color)-1;
		device->setWhite(false);
		device->showMedicBoxReady();
		device->showStatusText("Not started");
		device->preventSleep(10000);
	}
	
	virtual void processButton() {
	}
	
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			if(isMatchStarted && !isGameOver && cmd->shot_data.team_color != currentColor) {
				lastSwitchTime = DIV250(millis());
				previosColor = currentColor;
				currentColor = cmd->shot_data.team_color;
				updateMatchDisplay();
				device->setColor(currentColor);
				device->playConfirmBeep();
			}
		}
	}
	
	
	virtual void updateTime() {
		
		static unsigned oldTime = 0;
		unsigned currentTime = DIV250(millis());
		
		if ( oldTime != currentTime ) {
			oldTime = currentTime;
			
			if (!isGameStarted) {
				unsigned timeElapsed = currentTime - restartTime;
				timeLeft = startDelayTime - timeElapsed;
				
				if ( timeLeft < 24 ) {
					
					prestartCountdown(timeLeft);
					
					if (timeElapsed >= startDelayTime) {
						isGameStarted = true;
						gameStartTime = currentTime;
						device->playGameStarted();
						device->setWhite(true);
					}
				}
				
				if (timeElapsed % 4 == 0) {
					unsigned timeSeconds = timeLeft > 0 ? ((unsigned)timeLeft / 4) : 0;
					device->display.clearScreen();
					device->display.displayText(0, 0, " Not started ");
					device->display.displayTime(timeSeconds, 2);
				}
				
			}
			else if (!isMatchStarted) {
				
				unsigned timeElapsed = currentTime - gameStartTime;
				timeLeft = matchDelayTime - timeElapsed;
				
				if ( timeLeft > 0 ) {
					bool on = ((currentTime/2) % 2 == 0);
					device->setWhite(on);
					
					if (timeElapsed % 4 == 0) {
						unsigned timeSeconds = (unsigned)timeLeft / 4;
						device->display.clearScreen();
						device->display.displayText(0, 0, " - wait - ");
						device->display.displayTime(timeSeconds, 2);
					}
					
				} else {
					isMatchStarted = true;
					device->playGameOver();
					device->setWhite(true);
					device->showStatusText("Shot to start");
				}
				
				
			} else if ( lastSwitchTime != 0 ) {
				
				unsigned elapsed = currentTime - lastSwitchTime;
				prevTimeLeft = timeLeft;
				timeLeft = winTime - elapsed;
				
				if ( timeLeft > 0 ) {

					finalCountdown(timeLeft);
					updateMatchDisplay();
					
				} else {
					
					if (!isGameOver) {
						isGameOver = true;
						device->playGameOver();
						device->showGameOver();
					}
					
					bool on = (currentTime % 4 < 2);
					device->setColor( on ? currentColor : (mlt_team_color)-1 );
				}
				
			}
			device->preventSleep(10000);
		}
		
	}
	
	
	void updateMatchDisplay() {
		unsigned displayTime = timeLeft / 4;
		static unsigned oldDisplayTime = 0;
		if (displayTime != oldDisplayTime) {
			oldDisplayTime = displayTime;
			
			device->display.clearScreen();
			device->display.displayText(3, 0, colorTitles[currentColor]);
			device->display.displayTime(displayTime, 1);
			
			if ( (int)previosColor != -1 ) {
				const char prevTimeText[5];
				itoa(prevTimeText, prevTimeLeft, 10);
				device->display.displayText(0, 5, colorTitles[previosColor]);
				device->display.displayText(6, 5, prevTimeText);
			}
		}
	}
	
	void prestartCountdown(unsigned timeLeft) {
		uint8_t step = timeLeft / 8;
		uint8_t cnt = timeLeft % 8;
		uint8_t cmp = step + 3;
		if (cmp < cnt) {
			tone(SOUND_PIN, 500);
			device->setWhite(true);
		} else {
			noTone(SOUND_PIN);
			device->setWhite(false);
		}
	}
	
	void finalCountdown(unsigned timeLeft) {
		uint8_t progress = timeLeft < 45 ? (45 - timeLeft) : 0;
		uint8_t beepRepeat = progress / 32;

		static uint8_t cnt = 0;
		cnt = (cnt + 1) % 32; // 0..31
		
		uint8_t beepCycleNum = cnt / 4;
		if (beepCycleNum <= beepRepeat) {
			uint8_t phase = cnt % 4;
			if (phase < 2) {
				tone(SOUND_PIN, 500);
			} else {
				noTone(SOUND_PIN);
			}
		}
	}


protected:
	unsigned startDelayTime;
	unsigned matchDelayTime;
	unsigned winTime;
	
	unsigned restartTime;
	unsigned gameStartTime;
	unsigned lastSwitchTime;
	int timeLeft;
	int prevTimeLeft;
	bool isGameStarted;
	bool isMatchStarted;
	bool isGameOver;
	mlt_team_color currentColor;
	mlt_team_color previosColor;
};


class DominationTubeTDMSubmenu : public MedicBoxSubmenu
{
protected:
	uint8_t index;
	
public:
	DominationTubeTDMSubmenu(Device* aDevice, ConfigurationProfile* aConfig)
	: MedicBoxSubmenu(aDevice, aConfig) {
		index = config->getTdmParam() % (sizeof(tdmParams)/sizeof(tdmParams[0]));
	}
	
	virtual void reset() {
		updateDisplay();
		device->speaker.playBeep4();
	}
	
	virtual void processButton() {
		index = (index + 1) % (sizeof(tdmParams)/sizeof(tdmParams[0]));
		updateDisplay();
		delay(300);
	}
	
	virtual void saveConfig() {
		config->saveTdmParam(index);
	}
	
	void updateDisplay() {
		device->display.clearScreen();

		char buf[4];
		itoa(buf, tdmParams[index].startDelay, 10);
		device->display.displayText(1, 1, buf);
		device->display.displayText(4, 1, "wait");
		
		itoa(buf, tdmParams[index].matchDelay, 10);
		device->display.displayText(1, 2, buf);
		device->display.displayText(4, 2, "/");
		itoa(buf, tdmParams[index].matchDuration, 10);
		device->display.displayText(6, 2, buf);
	}
};


#endif /* MB_DOMINATION_TUBE_H */
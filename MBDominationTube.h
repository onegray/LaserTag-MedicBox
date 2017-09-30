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
					device->setRed(true);
				} else {
					device->setBlue(true);
				}
				
				updateDisplay();
				device->playConfirmBeep();
			}
		}
	}
	
	virtual void updateTime() {
		static int oldTime = 0;
		int currentTime = millis() / 1000;
		
		if ( oldTime != currentTime && (int)currentColor != -1 ) {
			oldTime = currentTime;
			
			int delta = currentTime - lastSwitchTime;
			int savedTime = currentColor == MLT_ST_RED ? savedTimeRed : savedTimeBlue;
			
			if ( savedTime + delta < winTime ) {
				
				updateDisplay();
				
			} else {
				
				if (!isGameOver) {
					isGameOver = true;
					device->playGameOver();
					device->showGameOver();
				}
				
				bool on = (currentTime % 2 == 0);
				if (currentColor == MLT_ST_RED) {
					device->setRed(on);
				} else {
					device->setBlue(on);
				}
			}
			device->preventSleep(10000);
		}
	}
	
	void updateDisplay() {
		int currentTime = millis() / 1000;
		int delta = currentTime - lastSwitchTime;
		int savedTime = currentColor == MLT_ST_RED ? savedTimeRed : savedTimeBlue;
		
		int time = savedTime + delta;
		const char* text = currentColor == MLT_ST_RED ? "  Red  " : "  Blue  ";
		
		device->display.clearScreen();
		device->display.displayText(0, 0, text);
		device->display.displayTime(time, 2);
	}
	
protected:
	int winTime;
	int lastSwitchTime;
	int savedTimeRed;
	int savedTimeBlue;
	bool isGameOver;
	mlt_team_color currentColor;
};



class DominationTubeTDM : public MedicBox
{
public:
	DominationTubeTDM(Device* aDevice, int winTimeSec)
	: MedicBox(aDevice) {
		this->startDelayTime = 30*(1000/64);
		this->readyDelayTime = 7*60*(1000/64);
		this->winTime = 40 * (1000/64);
	}
	
	virtual void reset() {
		isGameStarted = false;
		isGameReady = false;
		isGameOver = false;
		restartTime = millis() / 64;
		lastSwitchTime = 0;
		currentColor = (mlt_team_color)-1;
		device->setWhite(false);
		device->showMedicBoxReady();
		device->showStatusText("Not started");
		device->preventSleep(10000);
	}
	
	virtual void processButton() {
	}
	
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			if(isGameReady && !isGameOver && cmd->shot_data.team_color != currentColor) {
				
				lastSwitchTime = millis() / 64;
				
				currentColor = cmd->shot_data.team_color;
				
				if (currentColor == MLT_ST_RED) {
					device->setRed(true);
				} else {
					device->setBlue(true);
				}
				
				updateDisplay();
				device->playConfirmBeep();
			}
		}
	}
	
	virtual void updateTime() {
		static unsigned long oldTime = 0;
		unsigned long currentTime = millis() / 64;
		
		if ( oldTime != currentTime ) {
			oldTime = currentTime;
			
			if (!isGameStarted) {
				unsigned long timeElapsed = currentTime - restartTime;
				unsigned long timeLeft = startDelayTime - timeElapsed;
				
				if ( timeLeft < 10*(1000/64) ) {
					
					static int interval = 25;
					static int index = interval;
					static bool on = false;
					if(on) {
						if ( index-- == 0 ) {
							index = 1;
							on = false;
							tone(SOUND_PIN, 500);
							device->setWhite(true);
						}
					} else {
						index = interval;
						on = true;
						noTone(SOUND_PIN);
						device->setWhite(false);
					}
					
					if (timeElapsed >= startDelayTime) {
						noTone(SOUND_PIN);
						isGameStarted = true;
						device->playGameStarted();
						device->setWhite(true);
						//device->showStatusText("Game started!");
					}
				}
				
				if (timeElapsed % 8 == 0) {
					int timeSeconds = timeLeft * 64 / 1000;
					device->display.clearScreen();
					device->display.displayText(0, 0, " Not started ");
					device->display.displayTime(timeSeconds, 2);
				}
				
			}
			else if (!isGameReady) {
				
				unsigned long timeElapsed = currentTime - restartTime - startDelayTime;
				
				if ( timeElapsed < readyDelayTime ) {
					bool on = ((currentTime/2) % 2 == 0);
					device->setWhite(on);
					
					if (timeElapsed % 8 == 0) {
						unsigned long timeLeft = readyDelayTime - timeElapsed;
						int timeSeconds = timeLeft * 64 / 1000;
						device->display.clearScreen();
						device->display.displayText(0, 0, " - wait - ");
						device->display.displayTime(timeSeconds, 2);
					}
					
				} else {
					isGameReady = true;
					device->playGameOver();
					device->setWhite(true);
					//device->showStatusText("Shot to start");
				}
				
				
			} else if ( lastSwitchTime != 0 ) {
				
				
				unsigned long elapsed = currentTime - lastSwitchTime;
				
				if ( elapsed < winTime ) {
					
					
					unsigned char beepRepeat = elapsed / (30*(1000/64)/4);
					static unsigned char cnt = 0;
					cnt = (cnt + 1) % 32; // 0..31
					
					unsigned char beepCycleNum = cnt / 4;
					if (beepCycleNum <= beepRepeat) {
						unsigned char phase = cnt % 4;
						if (phase < 2) {
							tone(SOUND_PIN, 500);
							//device->setWhite(true);
						} else {
							noTone(SOUND_PIN);
							//device->setWhite(false);
						}
					}
					
					if (cnt % 8 == 0) {
						updateDisplay();
					}
					
				} else {
					
					if (!isGameOver) {
						isGameOver = true;
						device->playGameOver();
						device->showGameOver();
					}
					
					bool on = ((currentTime/4) % 2 == 0);
					if (currentColor == MLT_ST_RED) {
						device->setRed(on);
					} else {
						device->setBlue(on);
					}
				}
				
			}
			device->preventSleep(10000);
		}
		
	}
	
	void updateDisplay() {
		unsigned long currentTime = millis();
		unsigned long elapsed = currentTime - lastSwitchTime*64;
		int time = elapsed / 1000;
		const char* text = currentColor == MLT_ST_RED ? "  Red  " : "  Blue  ";

		device->display.clearScreen();
		device->display.displayText(0, 0, text);
		device->display.displayTime(time, 2);
	}
	
protected:
	int startDelayTime;
	int readyDelayTime;
	int winTime;
	
	unsigned long restartTime;
	unsigned long lastSwitchTime;
	bool isGameStarted;
	bool isGameReady;
	bool isGameOver;
	mlt_team_color currentColor;
};


#endif /* MB_DOMINATION_TUBE_H */

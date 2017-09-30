//
//  Device.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 7/4/15.
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

#ifndef DEVICE_H
#define DEVICE_H

#include "Display.h"
#include "Speaker.h"
#include "mlt_core.h"


class Device
{
public:
	
	Device();
	
	bool isButtonPressed();

	mlt_command receiveCommand();

	void sendNewGameCommand();
	
	void sendShotCommand();
	

	void showDeviceReady();
	
	void showMenuMode(const char* modeTitle);

	void showMedicBoxReady();
	
	void showTimeInterval(int ms, const char* title = NULL);
	
	void showTimerNumber(int number, const char* title = NULL);
	
	void showStatusText(const char* text);
	
	void showHealthNumber(int number);

	void showRespawnNumber(int number);
	
	void showHit();
	
	void showGameOver();
	
	void showRespawn();
	
	void showEmpty();

	void playConfirmBeep();

	void playWarningBeep();
	
	void playGameOver();
	
	void playGameStarted();
	
	void turnBacklight(bool mode);
	
	void setRed(bool on);
	void setBlue(bool on);
	void setWhite(bool on);

	void preventSleep(int duration);
	bool canSleep();
	
protected:
	unsigned long sleeplessEndingBound;
	bool backlightMode;
	
public:
	Display display;
	Speaker speaker;
};


#endif

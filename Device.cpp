//
//  Device.cpp
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


#include "Device.h"
#include "Circuit.h"
#include "Display.h"
#include "Speaker.h"
#include "mlt_core.h"

Device::Device() {
	sleeplessEndingBound = 0;
	backlightMode = false;
	
	pinMode(BTN_PIN, INPUT);
	pinMode(BACKLIGHT_PIN, OUTPUT);
	
	mltSetup(IR_EMITTER_PIN, IR_SENSOR_PIN);
}

bool Device::isButtonPressed() {
	return (digitalRead(BTN_PIN) == LOW);
}

mlt_command Device::receiveCommand() {
	return ::receiveCommand();
}

void Device::sendNewGameCommand() {
	::sendSystemCommand(MLT_SC_NEW_GAME);
}

void Device::sendShotCommand() {
	::sendShotCommand(0, MLT_ST_YELLOW, MLT_SHOT_DAMAGE_1);
}

void Device::showDeviceReady() {
	display.clearScreen();
	display.displayText(2, 2, "Device ready!");
	speaker.playComputer();
}

void Device::showMenuMode(const char* modeTitle) {
	display.clearScreen();
	display.displayText(14, 0, "- Mode -");
	display.displayText(0, 2, modeTitle);
	speaker.playBeep1();
}

void Device::showMedicBoxReady() {
	display.clearScreen();
	speaker.playBeep4();
}


void Device::showTimeInterval(int ms, const char* title) {
	display.clearScreen();
	if(title!=NULL) {
		display.displayText(0, 0, title);
	}
	display.displayFloating(ms, 2); 
}

void Device::showTimerNumber(int number, const char* title) {
	display.clearScreen();
	if(title!=NULL) {
		display.displayText(0, 0, title);
	}
	display.displayInteger(number, 2);
}

void Device::showStatusText(const char* text) {
	display.clearScreen();
	display.displayText(0, 1, text);
}


void Device::showHealthNumber(int number) {
	display.clearScreen();
	display.displayText(0, 0, "Health left:");
	display.displayInteger(number, 2);
}

void Device::showRespawnNumber(int number) {
	display.clearScreen();
	display.displayText(0, 0, "Resp number:");
	display.displayInteger(number, 2);
}

void Device::showHit() {
	display.clearScreen();
	display.displayHit();
	turnBacklight(!backlightMode);
	speaker.playRainbow3();
	turnBacklight(!backlightMode);
}

void Device::showGameOver() {
	display.clearScreen();
	display.displayHit();
	display.displayText(8, 3, "-----------");
	display.displayText(8, 4, " GAME OVER ");
	display.displayText(8, 5, "-----------");
	speaker.playMelody();
}

void Device::showRespawn() {
	//speaker.playBeep1();
}

void Device::showEmpty() {
	speaker.playBeep2();
}

void Device::playConfirmBeep() {
	speaker.playBeep1();
}

void Device::playWarningBeep() {
	speaker.playBeep2();
}


void Device::turnBacklight(bool mode) {
	backlightMode = mode;
	digitalWrite(BACKLIGHT_PIN, mode ? HIGH : LOW);
}

void Device::preventSleep(int duration) {
	unsigned long endingBound = millis() + duration;
	if(endingBound > sleeplessEndingBound) {
		sleeplessEndingBound = endingBound;
	}
}

bool Device::canSleep() {
	return millis() > sleeplessEndingBound;
}




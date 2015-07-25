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

#define SOUND_PIN    5

void Device::showTimeInterval(int ms, const char* title) {
	
	display.clearScreen();
	if(title!=NULL) {
		display.displayText(0, 0, title);
	}
	display.displayFloating(ms, 2);

	tone(SOUND_PIN, 200, 50);
	delay(60);
	noTone(SOUND_PIN);
	delay(50);
	tone(SOUND_PIN, 400, 100);
	delay(100);
	noTone(SOUND_PIN);
	
//	for (int i=0; i<20; i++) {
//		tone(SOUND_PIN, 200 + i*10, 10);
//		delay(12);
//		noTone(SOUND_PIN);
//	}
//	for (int i=20; i>=15; i--) {
//		tone(SOUND_PIN, 100 + i*10, 10);
//		delay(12);
//		noTone(SOUND_PIN);
//	}


}


void Device::showStatusText(const char* text) {
	display.clearScreen();
	display.displayText(0, 1, text);
}



void updateHealthNumber(int number) {
	
}

void updateResurectNumber(int number) {
	
}

void showHit() {
	
}

void showKilled() {
	
}

void showResurrect() {
	
}

void showEmpty() {
	
}





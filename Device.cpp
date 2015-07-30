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
#include "Speaker.h"

void Device::showStatusReady() {
	display.clearScreen();
	display.displayText(0, 2, "Device ready.");
	speaker.playComputer();
}


void Device::showTimeInterval(int ms, const char* title) {
	
	display.clearScreen();
	if(title!=NULL) {
		display.displayText(0, 0, title);
	}
	display.displayFloating(ms, 2);

}


void Device::showStatusText(const char* text) {
	display.clearScreen();
	display.displayText(0, 1, text);
}



void Device::updateHealthNumber(int number) {
	
}

void Device::updateResurectNumber(int number) {
	
}

void Device::showHit() {
	speaker.playRainbow2();
}

void Device::showKilled() {
	
}

void Device::showResurrect() {
	
}

void Device::showEmpty() {
	speaker.playBeep2();
}

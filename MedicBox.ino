//
//  MedicBox.ino
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

#include <Arduino.h>
#include <avr/sleep.h>

#include "Circuit.h"
#include "mlt_core.h"
#include "MedicBox.h"


static MedicBox* medic = NULL;

void setup() {

	pinMode(SOUND_PIN, OUTPUT);

	mltSetup(0, IR_SENSOR_PIN);

	medic = new MedicBox(NULL);
	
	delay(100);
}

void loop() {
	
	mlt_command cmd = receiveCommand();
	
	medic->processCommand(&cmd);

	Sleep();
}

void Sleep() {
	
	sleep_enable();
	attachInterrupt(0, onWakeUp, LOW);
	attachInterrupt(1, onWakeUp, LOW);
	set_sleep_mode(SLEEP_MODE_STANDBY);
	
	sleep_cpu();
	
	sleep_disable();
	detachInterrupt(0);
	detachInterrupt(1);
}


void onWakeUp() {
}






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


#ifndef MB_MEDIC_BOX_H
#define MB_MEDIC_BOX_H

#include "MBInterface.h"
#include "Device.h"

class SimpleMedicBox : public MedicBox
{
public:
	
	SimpleMedicBox(Device* aDevice)
	: MedicBox(aDevice) {
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
	
};

class RespCountMedicBox : public MedicBox
{
public:
	
	RespCountMedicBox(Device* aDevice)
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

#endif /* MB_MEDIC_BOX_H */

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

#import "mlt_core.h"
#import "Device.h"

class SmartMedic
{
protected:
	int health;
	int resurrectCount;
	
	int maxHealth;
	int maxResurrectCount;

	Device* device;
	
public:
	SmartMedic(Device* deviceParam) {
		device = deviceParam;
		resurrectCount = 0;
		maxResurrectCount = 10000;
		health = maxHealth = 10000;
	}

	SmartMedic(Device* deviceParam, int maxHealthParam, int maxResurrectCountParam) {
		device = deviceParam;
		resurrectCount = 0;
		maxResurrectCount = maxResurrectCountParam;
		health = maxHealth = maxHealthParam;
	}

	void reset() {
		health = maxHealth;
		resurrectCount = 0;
	}
	
	void sendResurrect() {
		if( health > 0 && resurrectCount < maxResurrectCount ) {
			sendSystemCommand(MLT_SC_NEW_GAME);
			resurrectCount++;
			device->updateResurectNumber(health);
		}
	}

	bool processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			if (health > 0) {
				health--;
				device->updateHealthNumber(health);
				if(health > 0) {
					device->showHit();
				} else {
					device->showKilled();
				}
			}
			return true;
		}
		if (cmd->command_type == MLT_CT_SYSTEM && cmd->sys_cmd == MLT_SC_NEW_GAME) {
			reset();
			return true;
		}
		return false;
	}
	
	bool isAlive() {
		return health > 0;
	}

	bool canSendResurrect() {
		return health > 0 && resurrectCount < maxResurrectCount;
	}
};

#endif

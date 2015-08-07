//
//  ModeMenu.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 8/1/15.
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

#ifndef MODE_MENU_H
#define MODE_MENU_H

#include "Device.h"
#include "MedicBox.h"
#include "ConfigurationProfile.h"

enum ModeType : char {
	ModeSimpleMedic,
	ModeStatisticMedic,
	ModeAliveMedic4,
	ModeAliveMedic10,
	ModeTestStunTime,
	ModeTestInvulnerabilityTime1300,
	ModeTestInvulnerabilityTime1310,
	ModeTestInvulnerabilityTime1320,
	ModeTestInvulnerabilityTime1350,
	
	ModeTestInvulnerabilityTime1500,
	ModeTestInvulnerabilityTime1520,
	ModeTestInvulnerabilityTime1550,
	ModeTypeCount,
};


class ModeMenu
{
public:
	
	ModeMenu(Device* aDevice, ConfigurationProfile* aConfig) {
		device = aDevice;
		config = aConfig;
		mode = (ModeType)config->getMenuMode();
		device->showMenuMode(getModeTitle(mode));
	}

	void changeMode() {
		mode = (ModeType) ((mode+1) % ModeTypeCount);
		device->showMenuMode(getModeTitle(mode));
	}
	
	void changeModeParam() {
		
	}

	void saveConfig() {
		config->saveMenuMode(mode);
	}

	
	static MedicBox* instantiateMedicBox(Device* device, ConfigurationProfile* config) {
		ModeType mode = (ModeType) config->getMenuMode();
		switch (mode) {
			case ModeSimpleMedic:
				return new MedicBox(device);
			case ModeStatisticMedic:
				return new StatisticMedicBox(device);
				
			case ModeAliveMedic4:
				return new AliveMedicBox(device, 4);
			case ModeAliveMedic10:
				return new AliveMedicBox(device, 10);
				
			case ModeTestStunTime:
				return new StunTimeTest(device);
				
			case ModeTestInvulnerabilityTime1300:
				return new InvulnerabilityTimeTest(device, 1300);
			case ModeTestInvulnerabilityTime1310:
				return new InvulnerabilityTimeTest(device, 1310);
			case ModeTestInvulnerabilityTime1320:
				return new InvulnerabilityTimeTest(device, 1320);
			case ModeTestInvulnerabilityTime1350:
				return new InvulnerabilityTimeTest(device, 1350);
				
			case ModeTestInvulnerabilityTime1500:
				return new InvulnerabilityTimeTest(device, 1500);
			case ModeTestInvulnerabilityTime1520:
				return new InvulnerabilityTimeTest(device, 1520);
			case ModeTestInvulnerabilityTime1550:
				return new InvulnerabilityTimeTest(device, 1550);
				
			default:
				break;
		}
		return NULL;
	}
	
	const char* getModeTitle(ModeType m) {
		static const char* modeTypeTitles[ModeTypeCount] = {
			"Simple",
			"Resp counting",
			
			"Alive Medic   health 4",
			"Alive Medic   health 10",
			
			"Test Stun Time",
			"Test Invulnerability 1.3",
			"Test Invulnerability 1.31",
			"Test Invulnerability 1.32",
			"Test Invulnerability 1.35",

			"Test Invulnerability 1.5",
			"Test Invulnerability 1.52",
			"Test Invulnerability 1.55",
		};
		return modeTypeTitles[m];
	}
	
protected:
	Device* device;
	ConfigurationProfile* config;
	ModeType mode;
};


#endif

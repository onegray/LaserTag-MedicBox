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
#include "ConfigurationProfile.h"
#include "MBSimpleMedic.h"
#include "MBSmartMedic.h"
#include "MBAnomalyPoint.h"
#include "MBDominationTube.h"
#include "MBTagTest.h"

class MedicBoxSubmenu;

enum ModeType : char {
	ModeSimpleMedic,
	ModeRespCountMedic,
	ModeAliveMedic4,
	ModeAliveMedic10,
	
	ModeTempMedic05_3,
	ModeTempMedic05_5,
	ModeTempMedic1_5,
	ModeTempMedic1_10,

	ModeTempAliveMedic2,
	ModeTempAliveMedic3,

	ModeAnomalyPoint,

	ModeDominationTube,
	ModeDominationTubeTDM,

	ModeTestStunTime,
	ModeTestInvulnerabilityTime,
	ModeTypeCount,
};

class ModeMenu
{
public:
	
	ModeMenu(Device* aDevice, ConfigurationProfile* aConfig) {
		device = aDevice;
		config = aConfig;
		mode = (ModeType)config->getMenuMode();
		backlight = config->getBacklightMode();
		device->showMenuMode(getModeTitle(mode));
		submenu = NULL;
	}

	~ModeMenu() {
		if (submenu) {
			delete submenu;
		}
	}
	
	void changeMode() {
		if (submenu) {
			delete submenu;
			submenu = NULL;
		}
		mode = (ModeType) ((mode+1) % ModeTypeCount);
		device->showMenuMode(getModeTitle(mode));
	}
	
	void changeModeParam() {
		
	}

	void changeBacklight() {
		backlight = (backlight+1) & 0x01;
		device->turnBacklight(backlight);
	}
	
	void saveConfig() {
		config->saveMenuMode(mode);
		config->saveBacklightMode(backlight);
		if (submenu) {
			submenu->saveConfig();
		}
	}

	
	static MedicBox* instantiateMedicBox(Device* device, ConfigurationProfile* config) {
		ModeType mode = (ModeType) config->getMenuMode();
		switch (mode) {
			case ModeSimpleMedic:
				return new SimpleMedicBox(device);
			case ModeRespCountMedic:
				return new RespCountMedicBox(device);
				
			case ModeAliveMedic4:
				return new AliveMedicBox(device, 4);
			case ModeAliveMedic10:
				return new AliveMedicBox(device, 10);
				
			case ModeTempMedic05_3:
				return new TempMedicBox(device, 0.5*60, 3*60);
			case ModeTempMedic05_5:
				return new TempMedicBox(device, 0.5*60, 5*60);
			case ModeTempMedic1_5:
				return new TempMedicBox(device, 1*60, 5*60);
			case ModeTempMedic1_10:
				return new TempMedicBox(device, 1*60, 10*60);
				
			case ModeTempAliveMedic2:
				return new TempAliveMedicBox(device, 2*60);
			case ModeTempAliveMedic3:
				return new TempAliveMedicBox(device, 3*60);

			case ModeAnomalyPoint:
				return new AnomalyPointMedicBox(device, config);
				
			case ModeDominationTube:
				return new DominationTube(device, 7*60);
			case ModeDominationTubeTDM:
				return new DominationTubeTDM(device, 40);
				
			case ModeTestStunTime:
				return new StunTimeTest(device);
				
			case ModeTestInvulnerabilityTime:
				return new InvulnerabilityTimeTest(device, config);
				
			default:
				break;
		}
		return NULL;
	}
	
	const char* getModeTitle(ModeType m) {
		switch (m) {
			case ModeSimpleMedic:
				return "Simple";
			case ModeRespCountMedic:
				return "Resp counting";
			case ModeAliveMedic4:
				return "Alive Medic   health 4";
			case ModeAliveMedic10:
				return "Alive Medic   health 10";
			case ModeTempMedic05_3:
				return "Temp Medic       0.5/3";
			case ModeTempMedic05_5:
				return "Temp Medic       0.5/5";
			case ModeTempMedic1_5:
				return "Temp Medic       1/5";
			case ModeTempMedic1_10:
				return "Temp Medic       1/10";
			case ModeTempAliveMedic2:
				return "Temp Medic      Alive/2";
			case ModeTempAliveMedic3:
				return "Temp Medic      Alive/3";
			case ModeAnomalyPoint:
				return "Anomaly Point *";
			case ModeDominationTube:
				return "Domination      Tube";
			case ModeDominationTubeTDM:
				return "Domination     Tube TDM";
			case ModeTestStunTime:
				return "Test Stun Time";
			case ModeTestInvulnerabilityTime:
				return "Test Invulnerability *";
			default:
				break;
		}
		return NULL;
	}
	
	bool instantiateSubmenu() {
		switch (mode) {
			case ModeAnomalyPoint:
				submenu = new AnomalyPointSubmenu(device, config);
				break;
			case ModeTestInvulnerabilityTime:
				submenu = new InvulnerabilityTestSubmenu(device, config);
				break;
			default:
				break;
		}
		return submenu != NULL;
	}
	
	MedicBoxSubmenu* submenu;
	
protected:
	Device* device;
	ConfigurationProfile* config;
	ModeType mode;
	bool backlight;
};


#endif

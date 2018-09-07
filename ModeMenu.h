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


#define MODE_SIMPLE_MEDIC           1
#define MODE_RESP_COUNT_MEDIC       1
#define MODE_ALIVE_MEDIC            1
#define MODE_TEMP_MEDIC             1
#define MODE_TEMP_ALIVE_MEDIC       1
#define MODE_ANOMALY_POINT          1
#define MODE_DOMINATION_TUBE        1
#define MODE_DOMINATION_TUBE_TDM    1
#define MODE_TEST_TAGGER            1


class MedicBoxSubmenu;

enum ModeType : char {

#if MODE_SIMPLE_MEDIC
	ModeSimpleMedic,
#endif

#if MODE_RESP_COUNT_MEDIC
	ModeRespCountMedic,
#endif

#if MODE_ALIVE_MEDIC
	ModeAliveMedic4,
	ModeAliveMedic10,
#endif

#if MODE_TEMP_MEDIC
	ModeTempMedic05_3,
	ModeTempMedic05_5,
	ModeTempMedic1_5,
	ModeTempMedic1_10,
	ModeTempMedic1_20,
	ModeTempMedic1_30,
	ModeTempMedicGreen1_30,
	ModeTempMedicRed1_30,

#endif

#if MODE_TEMP_ALIVE_MEDIC
	ModeTempAliveMedic2,
	ModeTempAliveMedic3,
#endif

#if MODE_ANOMALY_POINT
	ModeAnomalyPoint,
#endif

#if MODE_DOMINATION_TUBE
	ModeDominationTube,
#endif

#if MODE_DOMINATION_TUBE_TDM
	ModeDominationTubeTDM,
#endif

#if MODE_TEST_TAGGER
	ModeTestStunTime,
	ModeTestInvulnerabilityTime,
#endif

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
				
#if MODE_SIMPLE_MEDIC
			case ModeSimpleMedic:
				return new SimpleMedicBox(device);
#endif

#if MODE_RESP_COUNT_MEDIC
			case ModeRespCountMedic:
				return new RespCountMedicBox(device);
#endif

#if MODE_ALIVE_MEDIC
			case ModeAliveMedic4:
				return new AliveMedicBox(device, 4);
			case ModeAliveMedic10:
				return new AliveMedicBox(device, 10);
#endif

#if MODE_TEMP_MEDIC
			case ModeTempMedic05_3:
				return new TempMedicBox(device, 0.5*60, 3*60);
			case ModeTempMedic05_5:
				return new TempMedicBox(device, 0.5*60, 5*60);
			case ModeTempMedic1_5:
				return new TempMedicBox(device, 1*60, 5*60);
			case ModeTempMedic1_10:
				return new TempMedicBox(device, 1*60, 10*60);
			case ModeTempMedic1_20:
				return new TempMedicBox(device, 1*60, 20*60);
			case ModeTempMedic1_30:
				return new TempMedicBox(device, 1*60, 30*60);
			case ModeTempMedicGreen1_30:
				return new TempMedicBox(device, 1*60, 30*60);
			case ModeTempMedicRed1_30:
				return new TempMedicBox(device, 1*60, 30*60);

#endif

#if MODE_TEMP_ALIVE_MEDIC
			case ModeTempAliveMedic2:
				return new TempAliveMedicBox(device, 2*60);
			case ModeTempAliveMedic3:
				return new TempAliveMedicBox(device, 3*60);
#endif

#if MODE_ANOMALY_POINT
			case ModeAnomalyPoint:
				return new AnomalyPointMedicBox(device, config);
#endif

#if MODE_DOMINATION_TUBE
			case ModeDominationTube:
				return new DominationTube(device, config);
#endif

#if MODE_DOMINATION_TUBE_TDM
			case ModeDominationTubeTDM:
				return new DominationTubeTDM(device, config);
#endif

#if MODE_TEST_TAGGER
			case ModeTestStunTime:
				return new StunTimeTest(device);

			case ModeTestInvulnerabilityTime:
				return new InvulnerabilityTimeTest(device, config);
#endif

			default:
				break;
		}
		return NULL;
	}


	const char* getModeTitle(ModeType m) {
		switch (m) {
#if MODE_SIMPLE_MEDIC
			case ModeSimpleMedic:
				return "Simple";
#endif

#if MODE_RESP_COUNT_MEDIC
			case ModeRespCountMedic:
				return "Resp counting";
#endif

#if MODE_ALIVE_MEDIC
			case ModeAliveMedic4:
				return "Alive Medic   health 4";
			case ModeAliveMedic10:
				return "Alive Medic   health 10";
#endif

#if MODE_TEMP_MEDIC
			case ModeTempMedic05_3:
				return "Temp Medic       0.5/3";
			case ModeTempMedic05_5:
				return "Temp Medic       0.5/5";
			case ModeTempMedic1_5:
				return "Temp Medic       1/5";
			case ModeTempMedic1_10:
				return "Temp Medic       1/10";
			case ModeTempMedic1_20:
				return "Temp Medic       1/20";
			case ModeTempMedic1_30:
				return "Temp Medic       1/30";
			case ModeTempMedicGreen1_30:
				return "Temp Medic Green 1/30";
			case ModeTempMedicRed1_30:
				return "Temp Medic Red   1/30";

#endif

#if MODE_TEMP_ALIVE_MEDIC
			case ModeTempAliveMedic2:
				return "Temp Medic      Alive/2";
			case ModeTempAliveMedic3:
				return "Temp Medic      Alive/3";
#endif

#if MODE_ANOMALY_POINT
			case ModeAnomalyPoint:
				return "Anomaly Point *";
#endif

#if MODE_DOMINATION_TUBE
			case ModeDominationTube:
				return "Domination      Tube *";
#endif

#if MODE_DOMINATION_TUBE_TDM
			case ModeDominationTubeTDM:
				return "Domination     Tube TDM *";
#endif

#if MODE_TEST_TAGGER
			case ModeTestStunTime:
				return "Test Stun Time";
			case ModeTestInvulnerabilityTime:
				return "Test Invulnerability *";
#endif

			default:
				break;
		}
		return NULL;
	}

	bool instantiateSubmenu() {
		switch (mode) {
#if MODE_ANOMALY_POINT
			case ModeAnomalyPoint:
				submenu = new AnomalyPointSubmenu(device, config);
				break;
#endif

#if MODE_DOMINATION_TUBE
			case ModeDominationTube:
				submenu = new DominationTubeSubmenu(device, config);
				break;
#endif

#if MODE_DOMINATION_TUBE_TDM
			case ModeDominationTubeTDM:
				submenu = new DominationTubeTDMSubmenu(device, config);
				break;
#endif

#if MODE_TEST_TAGGER
			case ModeTestInvulnerabilityTime:
				submenu = new InvulnerabilityTestSubmenu(device, config);
				break;
#endif

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

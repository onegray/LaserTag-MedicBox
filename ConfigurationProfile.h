//
//  ConfigurationProfile.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 8/7/15.
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

#ifndef CONFIGURATION_PROFILE_H
#define CONFIGURATION_PROFILE_H

#include <EEPROM.h>
#include "mlt_core.h"

#define EEPROM_DATA_BASE_ADDR		0x20

struct EEPROM_DATA {
	uint8_t menuMode;
	uint8_t submenuMode;
	bool backlightMode;
	int menuParam;
	int invTime;
	mlt_command apCmd;
	uint8_t apShotInterval;
	int apStunTime;
	uint8_t dominationTubeParam;
	uint8_t tdmParam;
};

#define EEPROM_ADDR(field)	((int)&(((struct EEPROM_DATA*)EEPROM_DATA_BASE_ADDR)->field))

class ConfigurationProfile
{
public:

	ConfigurationProfile() {
		readData(&dataCache, sizeof(dataCache), EEPROM_DATA_BASE_ADDR);
	}
	
	uint8_t getMenuMode() {
		return dataCache.menuMode;
	}
	
	void saveMenuMode(uint8_t mode) {
		dataCache.menuMode = mode;
		writeData(&dataCache.menuMode, sizeof(dataCache.menuMode), EEPROM_ADDR(menuMode));
	}
	
	uint8_t getSubmenuMode() {
		return dataCache.submenuMode;
	}

	void saveSubmenuMode(uint8_t mode) {
		dataCache.submenuMode = mode;
		writeData(&dataCache.submenuMode, sizeof(dataCache.submenuMode), EEPROM_ADDR(submenuMode));
	}

	bool getBacklightMode() {
		return dataCache.backlightMode;
	}
	
	void saveBacklightMode(bool mode) {
		dataCache.backlightMode = mode;
		writeData(&dataCache.backlightMode, sizeof(dataCache.backlightMode), EEPROM_ADDR(backlightMode));
	}

	int getTestInvulnerabilityTime() {
		return dataCache.invTime;
	}

	void saveTestInvulnerabilityTime(int time) {
		dataCache.invTime = time;
		writeData(&dataCache.invTime, sizeof(dataCache.invTime), EEPROM_ADDR(invTime));
	}
	
	mlt_command getAnomalyPointCommand() {
		return dataCache.apCmd;
	}
	
	void saveAnomalyPointCommand(mlt_command* cmd) {
		dataCache.apCmd = *cmd;
		writeData(&dataCache.apCmd, sizeof(dataCache.apCmd), EEPROM_ADDR(apCmd));
	}
	
	uint8_t getAnomalyPointShotInterval() {
		return dataCache.apShotInterval;
	}
	
	void saveAnomalyPointShotInterval(uint8_t interval) {
		dataCache.apShotInterval = interval;
		writeData(&dataCache.apShotInterval, sizeof(dataCache.apShotInterval), EEPROM_ADDR(apShotInterval));
	}

	int getAnomalyPointStunTime() {
		return dataCache.apStunTime;
	}
	
	void saveAnomalyPointStunTime(int stunTime) {
		dataCache.apStunTime = stunTime;
		writeData(&dataCache.apStunTime, sizeof(dataCache.apStunTime), EEPROM_ADDR(apStunTime));
	}

	uint8_t getDominationTubeParam() {
		return dataCache.dominationTubeParam;
	}

	void saveDominationTubeParam(uint8_t param) {
		dataCache.dominationTubeParam = param;
		writeData(&dataCache.dominationTubeParam, sizeof(dataCache.dominationTubeParam), EEPROM_ADDR(dominationTubeParam));
	}

	uint8_t getTdmParam() {
		return dataCache.tdmParam;
	}

	void saveTdmParam(uint8_t param) {
		dataCache.tdmParam = param;
		writeData(&dataCache.tdmParam, sizeof(dataCache.tdmParam), EEPROM_ADDR(tdmParam));
	}


private:
	
	void readData(void* buf, int size, int addr) {
		for(int i=0; i<size; i++) {
			((uint8_t*)buf)[i] = EEPROM.read(addr+i);
		}
	}

	void writeData(const void* buf, int size, int addr) {
		for(int i=0; i<size; i++) {
			EEPROM.write(addr+i, ((uint8_t*)buf)[i]);
		}
	}
	
	EEPROM_DATA dataCache;
	
};


#endif

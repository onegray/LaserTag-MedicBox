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

#define EEPROM_DATA_BASE_ADDR		0x20

struct EEPROM_DATA {
	uint8_t menuMode;
	uint8_t submenuMode;
	bool backlightMode;
	int menuParam;
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

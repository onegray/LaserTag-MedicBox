//
//  SmartMedic.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 9/23/17.
//  Copyright (c) 2015-2017, Sergey Nikitenko. All rights reserved.
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


#ifndef MEDIC_BOX_H
#define MEDIC_BOX_H

#include "Device.h"
#include "ConfigurationProfile.h"

class MedicBox
{
public:

	MedicBox(Device* aDevice) {
		device = aDevice;
		config = NULL;
	}

	MedicBox(Device* aDevice, ConfigurationProfile* aConfig) {
		device = aDevice;
		config = aConfig;
	}
	
	virtual ~MedicBox() {
	}
	
	virtual void reset() {
	}
	
	virtual void processButton() {
	}
	
	virtual void processCommand(mlt_command* cmd) {
	}
	
	virtual void updateTime() {
	}
	
protected:
	Device* device;
	ConfigurationProfile* config;
};


class MedicBoxSubmenu
{
public:
	
	MedicBoxSubmenu(Device* aDevice, ConfigurationProfile* aConfig) {
		device = aDevice;
		config = aConfig;
	}
	
	virtual void reset() {
	}
	
	virtual void processButton() {
	}
	
	virtual void processCommand(mlt_command* cmd) {
	}
	
	virtual void saveConfig() {
	}
	
protected:
	Device* device;
	ConfigurationProfile* config;
};

#endif

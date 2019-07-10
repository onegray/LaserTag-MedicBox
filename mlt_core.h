//
//  mlt_core.h
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


#ifndef MLT_CORE_H
#define MLT_CORE_H

enum mlt_command_type : uint8_t {
	MLT_CT_INVALID = 0,
	MLT_CT_SHOT = 1,
	MLT_CT_SYSTEM = 0x83,
    MLT_CT_RESPAWN = 0xB4,//0xA9,
};

enum mlt_team_color : uint8_t {
	MLT_CLR_INVALID = (uint8_t)-1,
	MLT_CLR_RED = 0,
	MLT_CLR_BLUE = 1,
	MLT_CLR_YELLOW = 2,
	MLT_CLR_GREEN = 3,
};

enum mlt_shot_damage : uint8_t {
	MLT_SHOT_DAMAGE_1 = 0x00,
	MLT_SHOT_DAMAGE_2 = 0x01,
	MLT_SHOT_DAMAGE_4 = 0x02,
	MLT_SHOT_DAMAGE_5 = 0x03,
	MLT_SHOT_DAMAGE_7 = 0x04,
	MLT_SHOT_DAMAGE_10 = 0x05,
	MLT_SHOT_DAMAGE_15 = 0x06,
	MLT_SHOT_DAMAGE_17 = 0x07,
	MLT_SHOT_DAMAGE_20 = 0x08,
	MLT_SHOT_DAMAGE_25 = 0x09,
	MLT_SHOT_DAMAGE_30 = 0x0A,
	MLT_SHOT_DAMAGE_35 = 0x0B,
	MLT_SHOT_DAMAGE_40 = 0x0C,
	MLT_SHOT_DAMAGE_50 = 0x0D,
	MLT_SHOT_DAMAGE_75 = 0x0E,
	MLT_SHOT_DAMAGE_100 = 0x0F,
};

struct mlt_shot_data {
	char player_id;
	mlt_team_color team_color;
	mlt_shot_damage damage;
};

enum mlt_system_command : uint8_t {
	MLT_SC_ADMIN_KILL = 0x00,
	MLT_SC_PAUSE = 0x01,
	MLT_SC_START_GAME = 0x02,
	MLT_SC_RESET_SETTINGS = 0x03,
	MLT_SC_RESPAWN = 0x04,
	MLT_SC_NEW_GAME = 0x05,
	MLT_SC_FULL_AMMO = 0x06,
	MLT_SC_END_GAME = 0x07,
	MLT_SC_CHANGE_COLOR = 0x09, 	// LW extension
	MLT_SC_EXPLODE = 0x0B,
	MLT_SC_NEW_GAME_READY = 0x0C,
	MLT_SC_FULL_HEALTH = 0x0D,
	MLT_SC_CHANGE_DAMAGE = 0x0E,	// LW extension
	MLT_SC_TEST_SENSORS = 0x15,
	MLT_SC_DISARM = 0x17,
	MLT_SC_DOUBLE_HEALTH = 0x22,	// LW extension
};

/**
 * LASERWAR protocol supports different package ends.
 * Current values received from LW tech director https://vk.com/sempertenebris
 */
enum mlt_package_end : uint8_t {
	MLT_END_COMMON = 0xE8,
	MLT_END_RED = 0xB4,
	MLT_END_BLUE = 0xB5,
	MLT_END_YELLOW = 0xB6,
	MLT_END_GREEN = 0xB7,
};

struct mlt_command {

	/**
	 * First byte
	 */
	mlt_command_type command_type;

	union {
		mlt_shot_data shot_data;
		union {
			mlt_system_command sys_cmd;
			uint8_t cmd_data;
		};
	};

	/**
	 * Last byte, MLT_END_COMMON (0xE8) by default, c++ 11 feature
	 */
	mlt_package_end package_end = mlt_package_end::MLT_END_COMMON;
};

void mltSetup(uint8_t emitter_pin, uint8_t sensor_pin);

struct mlt_command receiveCommand();

void sendCommand(mlt_command cmd);

void sendSystemCommand(mlt_system_command cmd);

void sendShotCommand(char player_id, mlt_team_color color, mlt_shot_damage damage);

void printCommand(mlt_command cmd);

#endif

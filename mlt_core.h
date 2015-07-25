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

// Circuit settings
#define	IR_EMITTER_PIN  4
#define	IR_SENSOR_PIN   2


enum mlt_command_type : char {
	MLT_CT_INVALID = 0,
	MLT_CT_SHOT = 1,
	MLT_CT_SYSTEM = 2,
};

enum mlt_team_color : char {
	MLT_ST_RED = 0,
	MLT_ST_BLUE = 1,
	MLT_ST_YELLOW = 2,
	MLT_ST_GREEN = 3,
};

enum mlt_shot_damage : char {
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

enum mlt_system_command : char {
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

struct mlt_command {
	mlt_command_type command_type;
	union {
		mlt_shot_data shot_data;
		mlt_system_command sys_cmd;
	};
};


void mltSetup();

struct mlt_command receiveCommand();

void sendSystemCommand(mlt_system_command cmd);

void sendShotCommand(char player_id, mlt_team_color color, mlt_shot_damage damage);

void printCommand(mlt_command cmd);

#endif

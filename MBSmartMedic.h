//
//  MBSmartMedic.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 9/30/17.
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



#ifndef MB_SMART_MEDIC_H
#define MB_SMART_MEDIC_H

class AliveMedicBox : public MedicBox
{
public:
	
	AliveMedicBox(Device* aDevice, int healthParam)
	: MedicBox(aDevice) {
		health = maxHealth = healthParam;
	}

	virtual void reset() {
		health = maxHealth;
		device->showMedicBoxReady();
		device->showHealthNumber(health);
	}
	
	virtual void processButton() {
		if(health > 0) {
			sendSystemCommand(MLT_SC_NEW_GAME);
			device->showRespawn();
			delay(250);
		}
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(cmd->command_type == MLT_CT_SHOT) {
			if (health > 0) {
				health--;

				device->showHit();
				delay(300);
				
				if(health > 0) {
					device->showHealthNumber(health);
				} else {
					device->showGameOver();
				}
			}
		}
	}
	
protected:
	int health;
	int maxHealth;
};


class TempMedicBox : public MedicBox
{
public:
	
	TempMedicBox(Device* aDevice, int aliveTime, int stunTime)
	: MedicBox(aDevice) {
		this->aliveTime = aliveTime;
		//reason for it ? TODO very strange bug with changing stun time value
        this->stunTime = stunTime < 30*60 ? stunTime : 30*60;
	}
	
	virtual void reset() {
		alive = false;
		startTime = millis() / 1000;
		elapsedTime = 0;
		device->showMedicBoxReady();
		updateTime();
		device->preventSleep(2000);
	}
	
	virtual void processButton() {
		if(alive) {
			sendSystemCommand(MLT_SC_NEW_GAME);
			delay(300);
		}
		updateTime();
	}
	
	virtual void processCommand(mlt_command* cmd) {
	}

	virtual void updateTime() {
		unsigned long currentTime = millis() / 1000;
		int delta = currentTime - startTime;
		if( elapsedTime != delta ) {
			elapsedTime = delta;
			int timeLeft = (alive ? aliveTime : stunTime) - elapsedTime;
			if(timeLeft < 0) {
				alive = !alive;
				startTime = currentTime;
				timeLeft = (alive ? aliveTime : stunTime);
				device->playWarningBeep();
			}
			if(alive) {
				if(timeLeft < 60) {
					device->showTimerNumber(timeLeft, " Ready! ");
					device->playConfirmBeep();
				} else {
					device->showStatusText(" Ready! ");
				}
			} else {
				// minutes left (exclude current)
				device->showTimerNumber(timeLeft/60, " Waiting... ");
			}
			device->preventSleep(2000);
		}
	}

protected:
	int aliveTime;
	int stunTime;
	bool alive;
	int elapsedTime;
	unsigned long startTime;
};


class TempAliveMedicBox : public TempMedicBox
{
public:
	TempAliveMedicBox(Device* aDevice, int stunTime)
	: TempMedicBox(aDevice, 9999, stunTime) {
	}

	virtual void reset() {
		TempMedicBox::reset();
		alive = true;
		updateTime();
	}
	
	virtual void processCommand(mlt_command* cmd) {
		if(alive && cmd->command_type == MLT_CT_SHOT) {
			alive = false;
			startTime = millis() / 1000;
			device->showHit();
			updateTime();
		}
	}
};

#endif /* MB_SMART_MEDIC_H */

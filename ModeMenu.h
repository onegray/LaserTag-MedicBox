//
//  ModeMenu.h
//  MedicBox
//
//  Created by onegray on 8/1/15.
//
//

#ifndef MODE_MENU_H
#define MODE_MENU_H

#include "Device.h"
#include "MedicBox.h"

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
	ModeTestInvulnerabilityTime1510,
	ModeTestInvulnerabilityTime1520,
	ModeTestInvulnerabilityTime1550,
	ModeTypeCount,
};


class ModeMenu
{
public:
	
	ModeMenu(Device* aDevice) {
		device = aDevice;
		mode = ModeSimpleMedic;
		device->showMenuMode(getModeTitle(mode));
	}

	void changeMode() {
		mode = (ModeType) ((mode+1) % ModeTypeCount);
		device->showMenuMode(getModeTitle(mode));
	}
	
	void changeModeParam() {
		
	}
	
	MedicBox* instantiateMedicBox() {
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
			case ModeTestInvulnerabilityTime1510:
				return new InvulnerabilityTimeTest(device, 1510);
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
			"Test Invulnerability 1.51",
			"Test Invulnerability 1.52",
			"Test Invulnerability 1.55",
		};
		return modeTypeTitles[m];
	}
	
protected:
	Device* device;
	ModeType mode;
};


#endif

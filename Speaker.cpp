//
//  Speaker.cpp
//  MedicBox
//
//  Created by Sergey Nikitenko on 7/27/15.
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

#include "Speaker.h"

#define SOUND_PIN    5


static const PROGMEM int pitches[128] {
	8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 19, 21, 22, 23, 24, 26, 28, 29, 31, 33, 35, 37, 39, 41, 44, 46, 49, 52,
	55, 58, 62, 65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262,
	277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109,
	1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729,
	3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902, 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544
};


static const PROGMEM uint8_t dance_mix_score [] = {
	61,130,119, 61,130,119, 73,0,250, 61,5,119, 59,130,119, 59,130,119, 59,5,119, 71,5,119, 68,5,119, 71,5,119, 73,5,119, 61,130,119,
	61,130,119, 73,0,250, 61,5,119, 59,130,119, 59,5,119, 71,5,119, 59,5,119, 76,5,119, 75,0,125, 71,5,119, 73,5,119, 61,130,119,
	/*
	61,130,119, 73,0,250, 61,5,119, 59,130,119, 59,130,119, 59,5,119, 71,5,119, 68,5,119, 71,5,119, 73,5,119, 61,130,119, 61,130,119,
	73,0,250, 61,5,119, 59,130,119, 59,5,119, 71,5,119, 59,5,119, 76,5,119, 75,0,125, 71,5,119, 73,5,119, 61,0,499, 64,0,374, 61,0,249,
	61,0,124, 66,0,249, 61,0,249, 59,0,249, 61,0,499, 68,0,374, 61,0,249, 61,0,124, 69,0,249, 68,0,249, 64,0,249, 61,0,249, 68,0,250,
	73,0,249, 61,0,125, 59,0,249, 59,0,124, 56,0,249, 63,0,249, 61,0,249, 61,250,249, 37,255,119, 35,130,119, 35,5,119, 35,130,119,
	47,130,119, 35,130,119, 61,0,499, 64,0,374, 61,0,249, 61,0,124, 66,0,249, 61,0,249, 59,0,249, 61,0,499, 68,0,374, 61,0,249, 61,0,124,
	69,0,249, 68,0,249, 64,0,249, 61,0,249, 68,0,250, 73,0,249, 61,0,125, 59,0,249, 59,0,124, 56,0,249, 63,0,249, 61,0,249, 61,250,249,
	37,255,119, 35,130,119, 35,5,119, 35,130,119, 47,130,119, 35,130,119, 69,125,124, 72,125,125, 76,0,124, 72,125,125, 69,125,124,
	72,0,124, 35,5,119, 76,0,125, 47,5,119, 72,0,125, 69,125,124, 67,125,124, 72,125,125, 76,0,124, 72,125,125, 64,125,124, 72,0,124,
	35,5,119, 76,0,125, 47,5,119, 72,0,125, 64,125,124, 69,125,124, 72,125,125, 76,0,124, 72,125,125, 69,125,124, 72,0,124, 35,5,119,
	76,0,125, 47,5,119, 72,0,125, 69,125,124, 67,125,124, 72,125,125, 76,0,124, 72,125,125, 64,125,124, 72,0,124, 35,5,119, 76,0,125,
	47,5,119, 72,0,125, 64,125,124, 61,130,119, 61,130,119, 73,0,249, 61,5,119, 59,130,119, 59,5,119, 35,5,119, 59,5,119, 71,5,119,
	68,5,119, 71,5,119, 73,5,119, 61,130,119, 61,130,119, 73,0,249, 61,5,119, 59,130,119, 59,5,119, 71,5,119, 59,5,119, 76,5,119,
	75,0,125, 71,5,119, 73,5,119, 61,130,119, 61,130,119, 73,0,249, 61,5,119, 59,130,119, 59,5,119, 35,5,119, 59,5,119, 71,5,119,
	68,5,119, 71,5,119, 73,5,119, 61,130,119, 61,130,119, 73,0,249, 61,5,119, 59,130,119, 59,5,119, 71,5,119, 59,5,119, 76,5,119,
	75,0,125, 71,5,119, 73,5,119, 37,276,223, 37,187,187, 37,156,93, 37,67,57, 37,114,135, 49,109,140, 37,166,83, 34,296,203, 34,197,177,
	34,156,93, 34,78,46, 34,140,109, 46,114,135, 34,166,83, 29,265,234, 29,171,203, 29,130,119, 29,62,62, 29,135,114, 41,135,114,
	29,151,98, 29,359,140, 29,203,171, 29,140,109, 29,67,57, 29,156,93, 41,135,114, 29,125,125, 37,276,223, 37,187,187, 37,156,93,
	37,67,57, 37,114,135, 49,109,140, 37,166,83, 34,296,203, 34,197,177, 34,156,93, 34,78,46, 34,140,109, 46,114,135, 34,166,83,
	29,265,234, 29,171,203, 29,130,119, 29,62,62, 29,135,114, 41,135,114, 29,151,98, 29,109,140,
	*/ 
};

static void playScoreData(const uint8_t* data, int length)
{
	for (int i=0; i<length; i++)  {
		uint8_t note = pgm_read_byte(&data[i]) + 16;
		int toneFreq  = pgm_read_word(&pitches[note]);
		uint8_t pause = pgm_read_byte(&data[++i]);
		uint8_t noteDuration = pgm_read_byte(&data[++i]);
		tone(SOUND_PIN, toneFreq);
		delay(noteDuration);
		noTone(SOUND_PIN);
		delay(pause);
	}
}

void Speaker::playMelody() {
	playScoreData(dance_mix_score, sizeof(dance_mix_score));
}

void Speaker::playBeep1() {
	tone(SOUND_PIN, 500);
	delay(150);
	noTone(SOUND_PIN);
}

void Speaker::playBeep2() {
	tone(SOUND_PIN, 1200);
	delay(120);
	tone(SOUND_PIN, 900);
	delay(200);
	noTone(SOUND_PIN);
}

void Speaker::playBeep4() {
	for(int k = 0; k<4; k++) {
		tone(SOUND_PIN, 500, 60);
		delay(90);
	}
}

void Speaker::playBeep22() {
	for(int k = 0; k<2; k++) {
		tone(SOUND_PIN, 700, 70);
		delay(80);
		tone(SOUND_PIN, 500, 50);
		delay(60);
	}
}

void Speaker::playComputer() {
	static const PROGMEM uint8_t scoreData[] = { 59,90,109, 59,33,57, 59,80,93, 71,70,114 };
	playScoreData(scoreData, sizeof(scoreData));
}

void Speaker::playRainbow1() {
	for(int i = 0; i<8; i++) {
		int toneFreq  = pgm_read_word(&pitches[80-i]);
		tone(SOUND_PIN, toneFreq);
		delay(50-2*i);
		noTone(SOUND_PIN);
		delay(10-i);
	}
}

void Speaker::playRainbow2() {
	for(int k = 1; k<3; k++) {
		for(int i=k*5; i<(k+1)*10; i++) {
			tone(SOUND_PIN, 200+i*40);
			delay(10);
		}
		noTone(SOUND_PIN);
		delay(20);
	}
}

void Speaker::playRainbow3() {
	for(int k = 0; k<3; k++) {
		for(int i=0; i<10; i++) {
			tone(SOUND_PIN, 200+i*40 + k*100);
			delay(14);
		}
		for(int i=10; i>0; i--) {
			tone(SOUND_PIN, 200+i*40);
			delay(14-k);
		}
	}
	noTone(SOUND_PIN);
}

void Speaker::playRandom1() {
	for(int t = 5000; t<20000;t++) {
		int freq = t*(((t>>12)|(t>>8))&(63&(t>>4)));
		if(freq > 40 && freq < 2000) {
			tone(SOUND_PIN, freq);
			delay(2);
		}
	}
	noTone(SOUND_PIN);
}

void Speaker::playRandom2() {
	for(int t = 0; t<20000;t++) {
		int freq = 10*t*((t>>12)&(31&(t>>5)));
		if(freq > 40 && freq < 2000) {
			tone(SOUND_PIN, freq);
			delay(1);
		}
	}
	noTone(SOUND_PIN);
}


void Speaker::testAll() {
	
	playBeep1();
	delay(1000);

	playBeep2();
	delay(1000);

	playBeep4();
	delay(1000);
	
	playBeep22();
	delay(1000);

	playComputer();
	delay(1000);

	playRainbow1();
	delay(1000);

	playRainbow2();
	delay(1000);
	
	playRainbow3();
	delay(1000);
	
	playRandom1();
	delay(1000);
	
	playRandom2();
	delay(1000);

	playMelody();
}


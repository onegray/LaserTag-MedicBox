//
//  Utils.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 10/6/17.
//  Copyright (c) 2017, Sergey Nikitenko. All rights reserved.
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


#ifndef UTILS_H
#define UTILS_H

// Optimized division
#define DIV1000(x)	((x)*131/131072)
#define DIV500(x)	((x)*131/65536)
#define DIV250(x)	((x)*262/65536)
#define DIV100(x)	((x)*41/4096)
#define DIV60(x)	((x)*273/16384)
#define DIV10(x)	((x)*205/2048)


inline char* formatTime(unsigned secondsValue, char* buf) {
	uint8_t mins = DIV60((unsigned long)(secondsValue+1));
	uint8_t seconds = secondsValue - mins * 60;
	uint8_t secondsHi = DIV10((unsigned)seconds);
	uint8_t secondsLow = seconds - secondsHi * 10;
	buf[0] = 0x30 + mins;
	buf[1] = ':';
	buf[2] = 0x30 + secondsHi;
	buf[3] = 0x30 + secondsLow;
	buf[4] = 0;
}


#endif

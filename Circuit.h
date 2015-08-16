//
//  Circuit.h
//  MedicBox
//
//  Created by Sergey Nikitenko on 8/10/15.
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

#ifndef CIRCUIT_H
#define CIRCUIT_H


/* IR Sensor */
#define	IR_SENSOR_PIN   2	// Pulled-up input, active is "LOW"

/*
Vcc  100R
 + --/\/\--------------+
              TSOP4856 |
Vcc   1k      +-----+  |
 + --/\/\--+  |  O  |  |
           |  +-----+  |
Pin2       |   | | |   |
 o --------+---+ | +---+
                _|_
                 -     */


/* IR Emitter */
#define	IR_EMITTER_PIN  4	// Output
 
/*
Vcc    22R   led
 + ---/\/\--->|----+
                   |
Vcc    22R   led   |
 + ---/\/\--->|----+
                   |
            BD875  |k
              b__|/
Pin4  470R    |  |\
 o ---/\/\----+    |e
                  _|_
                   -   */


/* Push button */
#define	BTN_PIN         3	// Pulled-up input, active is "LOW"

/*
Vcc   1k
 + --/\/\--+
           |
Pin3       |
 o --------+
           |
            /  Btn
          _|_
           -          */



/* Speaker */
#define SOUND_PIN       5	// Output



/* Nokia 5110 LCD */
/* Pin mapping (defined in PCD8544_SPI.h):
 Pin8  - DC
 Pin9  - Reset
 Pin10 - CSE
 Pin11 - SDIN
 Pin13 - SCLK */

#define BACKLIGHT_PIN	6   // Output


#endif

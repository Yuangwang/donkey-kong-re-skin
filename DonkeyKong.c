// DonkeyKong.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 3/6/2015 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#define	 bowser 0
#define  mario  1
#define  fireball1  2
#define  fireball2  3
#define  fireball3  4
#define  fireball4  5
#define  fireball5  6
#define  character_size 7




// *************************** Images ***************************

const unsigned short mario_run_right[] = {
 0x7BEF, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x0000,
 0x3B93, 0xA7DF, 0x0000, 0x7BEF, 0xA9C5, 0xA9C5, 0xA9C5, 0xA9C5, 0x7BEF, 0x0000, 0x3B93, 0x0000, 0x0000, 0x7BEF, 0x0000, 0x3B93,
 0xA9C5, 0x3B93, 0x3B93, 0x9C46, 0x9C46, 0x9C46, 0x9C46, 0xA9C5, 0x0000, 0x3B93, 0x0000, 0x0000, 0x7BEF, 0x0000, 0x3B93, 0x3B93,
 0xFFFF, 0xFFFF, 0x3B93, 0x9C46, 0x9C46, 0x9C46, 0xE795, 0xA9C5, 0x0000, 0x3B93, 0x0000, 0x0000, 0x0000, 0x3B93, 0x3B93, 0xFFFF,
 0xFFFF, 0x3B93, 0x9C46, 0xFFFF, 0xFFFF, 0xE795, 0xFFFF, 0x0000, 0xA7DF, 0x0000, 0x0000, 0x7BEF, 0x0000, 0x3B93, 0xFFFF, 0xFFFF,
 0xFFFF, 0x3B93, 0xFFFF, 0xFFFF, 0xE795, 0xFFFF, 0xA9C5, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x7BEF, 0x3B93, 0x50D7, 0x50D7, 0x8B7D,
 0x3B93, 0x9C46, 0xE795, 0xE795, 0xA9C5, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x3B93, 0x50D7, 0x3B93, 0x3B93, 0x3B93,
 0x3B93, 0x3B93, 0xA9C5, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0x3B93, 0x3B93, 0x739F, 0x739F, 0x739F,
 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0x0000, 0x739F, 0x739F, 0xEF5F, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x1011, 0x0000, 0x739F, 0xEF5F, 0x0000, 0xEF5F, 0xEF5F, 0x0000, 0x739F, 0x739F,
 0x739F, 0x739F, 0x739F, 0x3B93, 0x7BEF, 0x1011, 0x739F, 0x3B93, 0xEF5F, 0x0000, 0x0000, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F,
 0xEF5F, 0xEF5F, 0x3B93, 0x7BEF, 0x1011, 0xEF5F, 0x3B93, 0xEF5F, 0x0000, 0xEF5F, 0xEF5F, 0x0000, 0xEF5F, 0x0000, 0xEF5F, 0x3B93,
 0x3B93, 0x7BEF, 0x7BEF, 0x7BEF, 0x1011, 0xEF5F, 0x0000, 0x0000, 0x0000, 0x739F, 0x0000, 0x739F, 0x0000, 0x739F, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x7BEF, 0x1011, 0x50D7, 0x50D7, 0x50D7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x7BEF, 0x7BEF, 0x7BEF, 0x1011, 0x50D7, 0x8B7D, 0x50D7, 0x50D7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x7BEF, 0x1011, 0x8B7D, 0x8B7D, 0x50D7, 0x50D7, 0x055B, 0xA7DF, 0xFFFF, 0x1011, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x7BEF, 0x1011, 0x1011, 0x8B7D, 0x8B7D, 0x8B7D, 0xA7DF, 0x8B7D, 0x1011, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x1011, 0x1011, 0x1011, 0x1011, 0x1011, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
};

const unsigned short mario_run_left[] = {
 0x7BEF, 0x7BEF, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0x0000, 0x7BEF, 0x7BEF,
 0x0000, 0x0000, 0x3B93, 0x0000, 0x7BEF, 0xA9C5, 0xA9C5, 0xA9C5, 0xA9C5, 0x7BEF, 0x0000, 0xA7DF, 0x3B93, 0x0000, 0x7BEF, 0x0000,
 0x0000, 0x3B93, 0x0000, 0xA9C5, 0x9C46, 0x9C46, 0x9C46, 0x9C46, 0x3B93, 0x3B93, 0xA9C5, 0x3B93, 0x0000, 0x0000, 0x0000, 0x3B93,
 0x0000, 0xA9C5, 0xE795, 0x9C46, 0x9C46, 0x9C46, 0x3B93, 0xFFFF, 0xFFFF, 0x3B93, 0x3B93, 0x0000, 0x0000, 0x0000, 0xA7DF, 0x0000,
 0xFFFF, 0xE795, 0xFFFF, 0xFFFF, 0x9C46, 0x3B93, 0xFFFF, 0xFFFF, 0x3B93, 0x3B93, 0x0000, 0x7BEF, 0x0000, 0x0000, 0xA9C5, 0xFFFF,
 0xE795, 0xFFFF, 0xFFFF, 0x3B93, 0xFFFF, 0xFFFF, 0xFFFF, 0x3B93, 0x0000, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0xA9C5, 0xE795,
 0xE795, 0x9C46, 0x3B93, 0x8B7D, 0x50D7, 0x50D7, 0x3B93, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0xA9C5, 0x3B93,
 0x3B93, 0x3B93, 0x3B93, 0x3B93, 0x50D7, 0x3B93, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x739F,
 0x739F, 0x739F, 0x3B93, 0x3B93, 0x0000, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0xEF5F, 0x739F, 0x739F, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x3B93, 0x739F, 0x739F, 0x739F, 0x739F, 0x739F, 0x0000, 0xEF5F, 0xEF5F,
 0x0000, 0xEF5F, 0x739F, 0x0000, 0x7BEF, 0x7BEF, 0x3B93, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0x0000, 0x0000,
 0xEF5F, 0x3B93, 0x739F, 0x1011, 0x7BEF, 0x7BEF, 0x3B93, 0x3B93, 0xEF5F, 0x0000, 0xEF5F, 0x0000, 0xEF5F, 0xEF5F, 0x0000, 0xEF5F,
 0x3B93, 0xEF5F, 0x1011, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x739F, 0x0000, 0x739F, 0x0000, 0x739F, 0x0000, 0x0000, 0x0000, 0x50D7,
 0x1011, 0x1011, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x50D7, 0x50D7, 0x50D7, 0x1011,
 0x7BEF, 0x7BEF, 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x50D7, 0x50D7, 0x8B7D, 0x50D7, 0x1011, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x7BEF, 0x1011, 0xFFFF, 0xA7DF, 0x055B, 0x50D7, 0x50D7, 0x8B7D, 0x8B7D, 0x1011, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x1011, 0x8B7D, 0xA7DF, 0x8B7D, 0x8B7D, 0x8B7D, 0x1011, 0x1011, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x1011, 0x1011, 0x1011, 0x1011, 0x1011, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
};

const unsigned short mario_still_right[] = {
 0x8410, 0x8410, 0x8410, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8410, 0x8410, 0x8410, 0x7BEF, 0x8410,
 0x8410, 0x8410, 0x0000, 0x3B93, 0x3B93, 0x3B93, 0xA7DF, 0x0000, 0xA7DF, 0x0000, 0x8410, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410,
 0x8410, 0x3B93, 0x3B93, 0x3B93, 0x3B93, 0x0000, 0x3B93, 0x0000, 0x8410, 0x8410, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410, 0x3B93,
 0xFFFF, 0xFFFF, 0x3B93, 0x9C46, 0x9C46, 0xA9C5, 0x9C46, 0xA9C5, 0x8410, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410, 0x3B93, 0xFFFF,
 0xFFFF, 0x3B93, 0x9C46, 0x9C46, 0x9C46, 0xE795, 0xE795, 0xA9C5, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410, 0x3B93, 0xFFFF, 0xFFFF,
 0xFFFF, 0x3B93, 0xFFFF, 0xFFFF, 0xE795, 0xFFFF, 0xA9C5, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410, 0x1011, 0x3B93, 0x3B93, 0x3B93,
 0x9C46, 0xFFFF, 0xFFFF, 0xE795, 0xFFFF, 0xA9C5, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410, 0x1011, 0x50D7, 0x50D7, 0x8B7D, 0x9C46,
 0x9C46, 0xE795, 0xE795, 0xA9C5, 0x8410, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410, 0x8410, 0x1011, 0x50D7, 0x3B93, 0x3B93, 0x3B93,
 0x3B93, 0xA9C5, 0x8410, 0x8410, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410, 0x0000, 0x3B93, 0x3B93, 0x739F, 0x739F, 0x739F, 0x0000,
 0x0000, 0x0000, 0x0000, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x0000, 0x0000, 0x739F, 0x739F, 0xEF5F, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x8410, 0x7BEF, 0x1011, 0x0000, 0x739F, 0xEF5F, 0x0000, 0xEF5F, 0xEF5F, 0x0000, 0x739F, 0x739F, 0x739F,
 0x739F, 0x739F, 0x3B93, 0x7BEF, 0x1011, 0x739F, 0x3B93, 0xEF5F, 0x0000, 0x0000, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F,
 0xEF5F, 0x3B93, 0x7BEF, 0x1011, 0xEF5F, 0x3B93, 0xEF5F, 0x0000, 0xFFFF, 0xEF5F, 0x0000, 0xEF5F, 0x0000, 0xEF5F, 0x3B93, 0x3B93,
 0x8410, 0x7BEF, 0x8410, 0x1011, 0xEF5F, 0x0000, 0x0000, 0x0000, 0x739F, 0x0000, 0x739F, 0x0000, 0x739F, 0x8410, 0x8410, 0x8410,
 0x7BEF, 0x8410, 0x1011, 0x50D7, 0x50D7, 0x50D7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF,
 0x8410, 0x8410, 0x1011, 0x50D7, 0x8B7D, 0x50D7, 0x50D7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8410, 0x7BEF, 0x8410,
 0x8410, 0x8410, 0x1011, 0x8B7D, 0x8B7D, 0x50D7, 0x50D7, 0x055B, 0xA7DF, 0xFFFF, 0x1011, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410,
 0x8410, 0x8410, 0x1011, 0x1011, 0x8B7D, 0x8B7D, 0x8B7D, 0xA7DF, 0x8B7D, 0x1011, 0x8410, 0x8410, 0x7BEF, 0x8410, 0x8410, 0x8410,
 0x8410, 0x8410, 0x8410, 0x1011, 0x1011, 0x1011, 0x1011, 0x1011, 0x8410, 0x8410, 0x8410, 0x7BEF,
};

const unsigned short mario_still_left[] = {
 0x8410, 0x8410, 0x8410, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8410, 0x8410, 0x7BEF, 0x7BEF, 0x8410,
 0x8410, 0x8410, 0x0000, 0xA7DF, 0x0000, 0xA7DF, 0x3B93, 0x3B93, 0x3B93, 0x0000, 0x8410, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410,
 0x8410, 0x8410, 0x0000, 0x3B93, 0x0000, 0x3B93, 0x3B93, 0x3B93, 0x3B93, 0x8410, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410, 0x8410,
 0xA9C5, 0x9C46, 0xA9C5, 0x9C46, 0x9C46, 0x3B93, 0xFFFF, 0xFFFF, 0x3B93, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410, 0xA9C5, 0xE795,
 0xE795, 0x9C46, 0x9C46, 0x9C46, 0x3B93, 0xFFFF, 0xFFFF, 0x3B93, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410, 0xA9C5, 0xFFFF, 0xE795,
 0xFFFF, 0xFFFF, 0x3B93, 0xFFFF, 0xFFFF, 0xFFFF, 0x3B93, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410, 0xA9C5, 0xFFFF, 0xE795, 0xFFFF,
 0xFFFF, 0x9C46, 0x3B93, 0x3B93, 0x3B93, 0x1011, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410, 0x8410, 0xA9C5, 0xE795, 0xE795, 0x9C46,
 0x9C46, 0x8B7D, 0x50D7, 0x50D7, 0x1011, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410, 0x8410, 0x8410, 0xA9C5, 0x3B93, 0x3B93, 0x3B93,
 0x3B93, 0x50D7, 0x1011, 0x8410, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410, 0x0000, 0x0000, 0x0000, 0x0000, 0x739F, 0x739F, 0x739F,
 0x3B93, 0x3B93, 0x0000, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xEF5F, 0x739F,
 0x739F, 0x0000, 0x0000, 0x7BEF, 0x7BEF, 0x3B93, 0x739F, 0x739F, 0x739F, 0x739F, 0x739F, 0x0000, 0xEF5F, 0xEF5F, 0x0000, 0xEF5F,
 0x739F, 0x0000, 0x1011, 0x7BEF, 0x3B93, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0x0000, 0x0000, 0xEF5F, 0x3B93,
 0x739F, 0x1011, 0x7BEF, 0x8410, 0x3B93, 0x3B93, 0xEF5F, 0x0000, 0xEF5F, 0x0000, 0xEF5F, 0xFFFF, 0x0000, 0xEF5F, 0x3B93, 0xEF5F,
 0x1011, 0x7BEF, 0x8410, 0x8410, 0x8410, 0x739F, 0x0000, 0x739F, 0x0000, 0x739F, 0x0000, 0x0000, 0x0000, 0xEF5F, 0x1011, 0x7BEF,
 0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x50D7, 0x50D7, 0x50D7, 0x1011, 0x7BEF, 0x7BEF,
 0x8410, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x50D7, 0x50D7, 0x8B7D, 0x50D7, 0x1011, 0x8410, 0x7BEF, 0x7BEF, 0x8410,
 0x8410, 0x1011, 0xFFFF, 0xA7DF, 0x055B, 0x50D7, 0x50D7, 0x8B7D, 0x8B7D, 0x1011, 0x8410, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410,
 0x1011, 0x8B7D, 0xA7DF, 0x8B7D, 0x8B7D, 0x8B7D, 0x1011, 0x1011, 0x8410, 0x8410, 0x8410, 0x7BEF, 0x7BEF, 0x8410, 0x8410, 0x8410,
 0x1011, 0x1011, 0x1011, 0x1011, 0x1011, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x7BEF, 0x7BEF,
};


const unsigned short bowser_open_mouth[] = {
 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF,
 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF,
 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0x451F, 0x451F, 0x451F, 0x451F, 0x0540, 0x0540, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540,
 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x451F, 0x0540, 0x0540, 0x451F, 0x0540, 0x0540,
 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x451F, 0x451F, 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540,
 0x0540, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540,
 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x451F, 0x451F, 0x0540, 0x0540, 0x0540, 0xFFFF,
 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF,
 0x0540, 0x0540, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x451F, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0xFFFF, 0x0540,
 0x0540, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0x0540, 0x451F,
 0x451F, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x451F, 0x451F, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x451F, 0x451F,
 0x451F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x451F, 0x451F,
 0x451F, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF,
 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF,
 0xFFFF, 0x0540, 0x0540, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0540, 0x451F, 0x451F, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540,
 0x0540, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x451F, 0x451F, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540,
 0x451F, 0x451F, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0540, 0x451F, 0x451F, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F,
 0xFFFF, 0x0540, 0x0540, 0x0000, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0540, 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x0540,
 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x0540, 0xFFFF,
 0x451F, 0x451F, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x0540,
 0x0540, 0x0540, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x0540, 0x0540, 0x0540,
 0xFFFF, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF,
 0x0540, 0x0540, 0x451F, 0xFFFF, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x0540,
 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x451F, 0x451F, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x451F, 0x451F, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000,
};

const unsigned short bowser_closed_mouth[] = {
 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF,
 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF,
 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0x451F, 0x451F, 0x451F, 0x451F, 0x0540, 0x0540, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540,
 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x451F, 0x0540, 0x0540, 0x451F, 0x0540, 0x0540,
 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x451F, 0x451F, 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540,
 0x0540, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540,
 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x451F, 0x451F, 0x0540, 0x0540, 0x0540, 0xFFFF,
 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF,
 0x0540, 0x0540, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x451F, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0xFFFF, 0x0540,
 0x0540, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0x0540, 0x451F,
 0x451F, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x451F, 0x451F, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x451F, 0x451F,
 0x451F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x451F, 0x451F,
 0x451F, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF,
 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF,
 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0540, 0x451F, 0x451F, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540,
 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x451F, 0x451F, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540,
 0x0540, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0540, 0x451F, 0x451F, 0xFFFF, 0xFFFF, 0xFFFF, 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540,
 0x451F, 0x451F, 0x0540, 0xFFFF, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0540, 0x451F, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F,
 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x0540, 0xFFFF,
 0x451F, 0x451F, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x451F, 0x451F, 0x0540,
 0x0540, 0x0540, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x451F, 0x0540, 0x0540, 0x0540,
 0xFFFF, 0xFFFF, 0xFFFF, 0x451F, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF,
 0x0540, 0x0540, 0x451F, 0xFFFF, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0xFFFF, 0x0540,
 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0540, 0x451F, 0x451F, 0x0540, 0x0540, 0x0540, 0x0540, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x451F, 0x451F, 0x0000, 0x0000, 0x0540, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x451F, 0x0000, 0x0000, 0x0000, 0x0540, 0x0540, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000,

};

const unsigned short ladder[] = {
 0xF733, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0xF733, 0xF733, 0xF733, 0xF733,
 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0xF733,
};

const unsigned short platform[] = {
 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x0000, 0x0000, 0x20FD, 0x0000, 0x20FD, 0x0000, 0x0000,
 0x20FD, 0x0000, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD,

};



const unsigned short fireball_right[] = {
 0x7BEF, 0x7BEF, 0x023C, 0x023C, 0x023C, 0x023C, 0x7BEF, 0x7BEF, 0x7BEF, 0x023C, 0x023C, 0x54BF, 0x54BF, 0x023C, 0x023C, 0x7BEF,
 0x7BEF, 0x023C, 0x54BF, 0x54BF, 0xFFFF, 0x54BF, 0x023C, 0x023C, 0x7BEF, 0x023C, 0x023C, 0x54BF, 0x54BF, 0xFFFF, 0x54BF, 0x023C,
 0x7BEF, 0x7BEF, 0x023C, 0x023C, 0x023C, 0x54BF, 0x54BF, 0x023C, 0x023C, 0x7BEF, 0x7BEF, 0x7BEF, 0x023C, 0x54BF, 0x023C, 0x023C,
 0x7BEF, 0x7BEF, 0x023C, 0x7BEF, 0x023C, 0x023C, 0x023C, 0x7BEF, 0x7BEF, 0x023C, 0x7BEF, 0x023C, 0x023C, 0x7BEF, 0x7BEF, 0x7BEF,
};

const unsigned short fireball_left[] = {
 0x7BEF, 0x7BEF, 0x023C, 0x023C, 0x023C, 0x023C, 0x7BEF, 0x7BEF, 0x7BEF, 0x023C, 0x023C, 0x54BF, 0x54BF, 0x023C, 0x023C, 0x7BEF,
 0x023C, 0x023C, 0x54BF, 0xFFFF, 0x54BF, 0x54BF, 0x023C, 0x7BEF, 0x023C, 0x54BF, 0xFFFF, 0x54BF, 0x54BF, 0x023C, 0x023C, 0x7BEF,
 0x023C, 0x54BF, 0x54BF, 0x023C, 0x023C, 0x023C, 0x7BEF, 0x7BEF, 0x023C, 0x023C, 0x54BF, 0x023C, 0x7BEF, 0x7BEF, 0x7BEF, 0x023C,
 0x7BEF, 0x023C, 0x023C, 0x023C, 0x7BEF, 0x023C, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x023C, 0x023C, 0x7BEF, 0x023C, 0x7BEF,
};



// *************************** Capture image dimensions out of BMP**********

//**********************Character Status Typedef****************************
typedef enum {dead,alive,open,closed} state;
typedef enum {still,walking,jumping,climbing_up,climbing_down} moving;
typedef enum {left,right} direct;
typedef struct charac{
	const unsigned short *pic;
	state status;
	moving movement;
	direct direction;
	uint32_t pastx;
	uint32_t pasty;
	uint32_t newx;
	uint32_t newy;
	int8_t changex;
	int8_t changey;
} character;
typedef struct coord{
	uint32_t x;
	uint32_t y;
} coordinates;
//*******************Character Status***************************************
character characters[character_size] = {																	//this array has all of the characters in the game
	{bowser_open_mouth,open,still,right,5,32,5,32,0,0},				//bowser status
	{mario_still_left,alive,still,right,109,153,109,153,-2,2},		//mario status
	{fireball_right,alive,walking,right,40,153,40,153,2,0},		//fireball 1 status
	{fireball_right,dead,walking,right,40,153,40,153,2,0},		//fireball 2 status
	{fireball_right,dead,walking,right,40,153,40,153,2,0},		//fireball 3 status
	{fireball_right,dead,walking,right,40,153,40,153,2,0},		//fireball 4 status
	{fireball_right,dead,walking,right,40,153,40,153,2,0},		//fireball 5 status
};
//ladders
coordinates enemy_ladders_bottom[24]={
	{20,151},{19,151},{21,151},{74,153},{75,153},{76,153},	//row1
	{44,118},{45,118},{46,118},{89,117},{90,117},{91,117},	//row2
	{17,88},{18,88},{19,88},{59,90},{60,90},{61,90},				//row3
	{41,60},{42,60},{43,60},{101,62},{102,62},{103,62},			//row4
	//enemies cannot go up to win level
};
coordinates player_ladders_bottom[18]={
	{20,151},{19,151},{21,151},															//row1
	{44,118},{45,118},{46,118},{89,117},{90,117},{91,117},	//row2
	{17,88},{18,88},{19,88},																//row3
	{101,62},{102,62},{103,62},															//row4
	{69,34},{70,34},{71,34},																//row5
};
coordinates enemy_ladders_top[24]={
	{20,119},{19,119},{21,119},{74,120},{75,120},{76,120},	//row2
	{44,89},{45,89},{46,89},{89,91},{90,91},{91,91},				//row3
	{17,63},{18,63},{19,63},{59,61},{60,61},{61,61},				//row4
	{41,33},{42,33},{43,33},{101,35},{102,35},{103,35},			//row5
	//enemies cannot go up to win level
};
coordinates player_ladders_top[18]={
	{20,119},{19,119},{21,119},															//row2
	{44,89},{45,189},{46,89},{89,91},{90,91},{91,91},				//row3
	{17,63},{18,63},{19,63},																//row4
	{101,35},{102,35},{103,35},															//row5
	{69,17},{70,17},{71,17}
};

//function declarations
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
void Move(uint8_t char_num);
uint32_t x_ADC_In(void);
uint32_t y_ADC_In(void);

int main(void){
  TExaS_Init();  // set system clock to 80 MHz
  Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black

//draw map
//bottom row
	for(uint32_t i=0,x=0;i<6;i++,x+=6){
		ST7735_DrawBitmap(x,156,platform,6,5);
	}
	for(uint32_t i=0,x=36;i<6;i++,x+=6){
		ST7735_DrawBitmap(x,157,platform,6,5);
	}
	for(uint32_t i=0,x=72;i<10;i++,x+=6){
		ST7735_DrawBitmap(x,158,platform,6,5);
	}
//2nd row
	for(uint32_t i=0,x=12;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,124,platform,6,5);
	}
	for(uint32_t i=0,x=42;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,123,platform,6,5);
	}
	for(uint32_t i=0,x=72;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,122,platform,6,5);
	}
	for(uint32_t i=0,x=102;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,121,platform,6,5);
	}
//3rd row
	for(uint32_t i=0,x=0;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,93,platform,6,5);
	}
	for(uint32_t i=0,x=30;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,94,platform,6,5);
	}
	for(uint32_t i=0,x=60;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,95,platform,6,5);
	}
	for(uint32_t i=0,x=90;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,96,platform,6,5);
	}
//4th row
	for(uint32_t i=0,x=12;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,68,platform,6,5);
	}
	for(uint32_t i=0,x=42;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,67,platform,6,5);
	}
	for(uint32_t i=0,x=72;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,66,platform,6,5);
	}
	for(uint32_t i=0,x=102;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,65,platform,6,5);
	}
//top row
	for(uint32_t i=0,x=0;i<10;i++,x+=6){
		ST7735_DrawBitmap(x,38,platform,6,5);
	}
	for(uint32_t i=0,x=60;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,39,platform,6,5);
	}
	for(uint32_t i=0,x=90;i<5;i++,x+=6){
		ST7735_DrawBitmap(x,40,platform,6,5);
	}
//level complete row
	for(uint32_t i=0,x=50;i<8;i++,x+=6){
		ST7735_DrawBitmap(x,22,platform,6,5);
	}
//initial drawing of mario
	ST7735_DrawBitmap(characters[mario].newx, characters[mario].newy, characters[mario].pic, 15,20); 
//initial drawing of bowser
	ST7735_DrawBitmap(characters[bowser].newx, characters[bowser].newy, characters[bowser].pic, 31,31);
//ladders
//first row ladders
	for(uint32_t i=0,y=151;i<9;i++,y-=3){
		ST7735_DrawBitmap(20,y,ladder,13,3);
	}
	for(uint32_t i=0,y=128;i<2;i++,y-=3){
		ST7735_DrawBitmap(75,y,ladder,13,3);
	}
	for(uint32_t i=0,y=153;i<5;i++,y-=3){
		ST7735_DrawBitmap(75,y,ladder,13,3);
	}
//second row ladders
	for(uint32_t i=0,y=118;i<8;i++,y-=3){
		ST7735_DrawBitmap(45,y,ladder,13,3);
	}
	for(uint32_t i=0,y=117;i<7;i++,y-=3){
		ST7735_DrawBitmap(90,y,ladder,13,3);
	}
//third row ladders
	for(uint32_t i=0,y=88;i<7;i++,y-=3){
		ST7735_DrawBitmap(18,y,ladder,13,3);
	}
	for(uint32_t i=0,y=90;i<3;i++,y-=3){
		ST7735_DrawBitmap(60,y,ladder,13,3);
	}
	for(uint32_t i=0,y=75;i<3;i++,y-=3){
		ST7735_DrawBitmap(60,y,ladder,13,3);
	}
//fourth row ladders
	for(uint32_t i=0,y=50;i<4;i++,y-=3){
		ST7735_DrawBitmap(42,y,ladder,13,3);
	}
	for(uint32_t i=0,y=62;i<2;i++,y-=3){
		ST7735_DrawBitmap(42,y,ladder,13,3);
	}
	for(uint32_t i=0,y=60;i<7;i++,y-=3){
		ST7735_DrawBitmap(102,y,ladder,13,3);
	}
//win game ladder
	for(uint32_t i=0,y=34;i<4;i++,y-=3){
		ST7735_DrawBitmap(70,y,ladder,13,3);
	}



  while(1){
		if(characters[bowser].status==open){
			characters[bowser].status=closed;
			characters[bowser].pic = bowser_open_mouth;
		}
		else{
			characters[bowser].status = open;
			characters[bowser].pic = bowser_closed_mouth;
		}
		Delay100ms(1);
		Move(mario);
		ST7735_DrawBitmap(characters[bowser].newx, characters[bowser].newy, characters[bowser].pic, 31,31);
		ST7735_DrawBitmap(characters[mario].newx, characters[mario].newy, characters[mario].pic, 15,20); 
  }
}

void Move(uint8_t char_num){
	characters[char_num].pastx = characters[char_num].newx;
	characters[char_num].pasty = characters[char_num].newy;
	characters[char_num].newx += characters[char_num].changex;
	characters[char_num].newy -= characters[char_num].changey;		//this flips it so positive changey goes up on the screen
}

void checkADC (void){
	uint32_t xADCvalue = x_ADC_In();
	uint32_t yADCvalue = y_ADC_In();
	if((xADCvalue<=2200)&&(xADCvalue>=1900)){
		characters[mario].changex = 0;
	}
	if(xADCvalue<1900){
		characters[mario].changex = -2;
	}
	if(xADCvalue>2200){
		characters[mario].changex = 2;
	}
	if((yADCvalue<2200)&&(yADCvalue>1900)){
		characters[mario].changex = 0;
	}
	if(yADCvalue<1900){
		//checks if it is by a ladder
	}
	if(yADCvalue>2200){
		//checks if it is by a ladder
	}
}

// You can use this timer only if you learn how it works

void Delay100ms(uint32_t count){
	uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

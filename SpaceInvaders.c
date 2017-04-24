// SpaceInvaders.c
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


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

/*
// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10
const unsigned short SmallEnemy10pointA[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// enemy ship that starts at the top of the screen (arms/mouth open)
// width=16 x height=10
const unsigned short SmallEnemy10pointB[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// enemy ship that starts in the middle of the screen (arms together)
// width=16 x height=10
const unsigned short SmallEnemy20pointA[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// enemy ship that starts in the middle of the screen (arms apart)
// width=16 x height=10
const unsigned short SmallEnemy20pointB[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};



// enemy ship that starts at the bottom of the screen (arms down)
// width=16 x height=10
const unsigned short SmallEnemy30pointA[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// enemy ship that starts at the bottom of the screen (arms up)
// width=16 x height=10
const unsigned short SmallEnemy30pointB[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

// image of the player's ship
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 x height=8
const unsigned short PlayerShip0[] = {
 0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0,
 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0,
 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,


};

// small shield floating in space to cover the player's ship from enemy fire (undamaged)
// width=18 x height=5
const unsigned short Bunker0[] = {
 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x079F, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x0000, 0x0000, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F,
 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x079F, 0x0000, 0x0000,

};
*/
const unsigned short mario_run[] = {
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
 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x1011, 0x1011, 0x1011, 0x1011, 0x1011, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,

};
const unsigned short mario_still[] = {
 0x8410, 0x8410, 0x8410, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410,
 0x8410, 0x0000, 0x3B93, 0x3B93, 0x3B93, 0xA7DF, 0x0000, 0xA7DF, 0x0000, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x3B93,
 0x3B93, 0x3B93, 0x3B93, 0x0000, 0x3B93, 0x0000, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x3B93, 0xFFFF, 0xFFFF, 0x3B93,
 0x9C46, 0x9C46, 0xA9C5, 0x9C46, 0xA9C5, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x3B93, 0xFFFF, 0xFFFF, 0x3B93, 0x9C46, 0x9C46,
 0x9C46, 0xE795, 0xE795, 0xA9C5, 0x8410, 0x8410, 0x8410, 0x8410, 0x3B93, 0xFFFF, 0xFFFF, 0xFFFF, 0x3B93, 0xFFFF, 0xFFFF, 0xE795,
 0xFFFF, 0xA9C5, 0x8410, 0x8410, 0x8410, 0x8410, 0x1011, 0x3B93, 0x3B93, 0x3B93, 0x9C46, 0xFFFF, 0xFFFF, 0xE795, 0xFFFF, 0xA9C5,
 0x8410, 0x8410, 0x8410, 0x8410, 0x1011, 0x50D7, 0x50D7, 0x8B7D, 0x9C46, 0x9C46, 0xE795, 0xE795, 0xA9C5, 0x8410, 0x8410, 0x8410,
 0x8410, 0x8410, 0x8410, 0x1011, 0x50D7, 0x3B93, 0x3B93, 0x3B93, 0x3B93, 0xA9C5, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410,
 0x0000, 0x3B93, 0x3B93, 0x739F, 0x739F, 0x739F, 0x0000, 0x0000, 0x0000, 0x0000, 0x8410, 0x8410, 0x8410, 0x0000, 0x0000, 0x739F,
 0x739F, 0xEF5F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8410, 0x1011, 0x0000, 0x739F, 0xEF5F, 0x0000, 0xEF5F,
 0xEF5F, 0x0000, 0x739F, 0x739F, 0x739F, 0x739F, 0x739F, 0x3B93, 0x1011, 0x739F, 0x3B93, 0xEF5F, 0x0000, 0x0000, 0xEF5F, 0xEF5F,
 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0xEF5F, 0x3B93, 0x1011, 0xEF5F, 0x3B93, 0xEF5F, 0x0000, 0xFFFF, 0xEF5F, 0x0000, 0xEF5F, 0x0000,
 0xEF5F, 0x3B93, 0x3B93, 0x8410, 0x8410, 0x1011, 0xEF5F, 0x0000, 0x0000, 0x0000, 0x739F, 0x0000, 0x739F, 0x0000, 0x739F, 0x8410,
 0x8410, 0x8410, 0x8410, 0x1011, 0x50D7, 0x50D7, 0x50D7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x8410, 0x8410, 0x1011, 0x50D7, 0x8B7D, 0x50D7, 0x50D7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8410, 0x8410, 0x8410,
 0x8410, 0x1011, 0x8B7D, 0x8B7D, 0x50D7, 0x50D7, 0x055B, 0xA7DF, 0xFFFF, 0x1011, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410,
 0x1011, 0x1011, 0x8B7D, 0x8B7D, 0x8B7D, 0xA7DF, 0x8B7D, 0x1011, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410,
 0x1011, 0x1011, 0x1011, 0x1011, 0x1011, 0x8410, 0x8410, 0x8410,

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
 0x451F, 0x451F, 0x0540, 0x0540, 0x451F, 0x451F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
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
 0xF733, 0xF733, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF733, 0xF733, 0xF733, 0xF733, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733,
 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF733, 0xF755,
 0xF755, 0xF755, 0xF733, 0xF733, 0xF733, 0xF733, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF733,
 0xF733, 0xF733, 0xF733, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF733, 0xF733,

};

const unsigned short platform[] = {
 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x0000, 0x0000, 0x20FD, 0x0000, 0x20FD, 0x0000, 0x0000,
 0x20FD, 0x0000, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x0000, 0x0000, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD, 0x20FD,

};



// *************************** Capture image dimensions out of BMP**********

int main(void){
  TExaS_Init();  // set system clock to 80 MHz
  Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black

//draw map
//bottom row
	for(uint32_t i=0,x=0;i<10;i++,x+=6){
	ST7735_DrawBitmap(x,158,platform,6,5);
	}
	for(uint32_t i=0,x=60;i<6;i++,x+=6){
	ST7735_DrawBitmap(x,156,platform,6,5);
	}
	for(uint32_t i=0,x=96;i<6;i++,x+=6){
	ST7735_DrawBitmap(x,154,platform,6,5);
	}
//2nd row
	for(uint32_t i=0,x=0;i<5;i++,x+=6){
	ST7735_DrawBitmap(x,110,platform,6,5);
	}
	for(uint32_t i=0,x=30;i<5;i++,x+=6){
	ST7735_DrawBitmap(x,112,platform,6,5);
	}
	for(uint32_t i=0,x=60;i<5;i++,x+=6){
	ST7735_DrawBitmap(x,114,platform,6,5);
	}
	for(uint32_t i=0,x=90;i<5;i++,x+=6){
	ST7735_DrawBitmap(x,116,platform,6,5);
	}
//3rd row
	for(uint32_t i=0,x=12;i<5;i++,x+=6){
	ST7735_DrawBitmap(x,76,platform,6,5);
	}
	for(uint32_t i=0,x=42;i<5;i++,x+=6){
	ST7735_DrawBitmap(x,74,platform,6,5);
	}
	for(uint32_t i=0,x=72;i<5;i++,x+=6){
	ST7735_DrawBitmap(x,72,platform,6,5);
	}
	for(uint32_t i=0,x=102;i<5;i++,x+=6){
	ST7735_DrawBitmap(x,70,platform,6,5);
	}
//top row
	for(uint32_t i=0,x=0;i<22;i++,x+=6){
	ST7735_DrawBitmap(x,45,platform,6,5);
	}

//initial drawing of mario
	ST7735_DrawBitmap(20, 153, mario_still, 14,20); 
//initial drawing of bowser
	ST7735_DrawBitmap(15, 40, bowser_closed_mouth, 31,31);
//testing other bowser	
	Delay100ms(5);              // delay 5 sec at 80 MHz
	ST7735_DrawBitmap(15, 40, bowser_open_mouth, 31,31);
	Delay100ms(5);              // delay 5 sec at 80 MHz
	ST7735_DrawBitmap(15, 40, bowser_closed_mouth, 31,31);	
	Delay100ms(5);              // delay 5 sec at 80 MHz
	ST7735_DrawBitmap(15, 40, bowser_open_mouth, 31,31);
//testing  
//  ST7735_DrawBitmapClear(30, 30, mario_run, 15,19); 
//	ST7735_DrawBitmap(60, 60, mario_still, 14,20); 
//	ST7735_DrawBitmap(90, 90, platform, 6,5); 


  Delay100ms(50);              // delay 5 sec at 80 MHz


  ST7735_FillScreen(0x0000);            // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);
  while(1){
  }

}


// You can use this timer only if you learn how it works

void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

/*
	**the openSX70 project**
	
   It is many things at once, but simply put, openSX70 is an open source 
  (hardware and software) project that aims to take the SX70 beyond what
  is possible now in a cheap and non destructive way.
	https://opensx70.com/
	
	https://github.com/openSX70
	
  As a legal reminder please note that the code and files is under Creative Commons 
  "Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)" is free and open 
  for hobbyist NON-COMMERCIAL USE.
	https://creativecommons.org/licenses/by-nc/4.0/
	
	You are free to:
	Share — copy and redistribute the material in any medium or format
	Adapt — remix, transform, and build upon the material
	The licensor cannot revoke these freedoms as long as you follow the license terms.
	Under the following terms:
	Attribution — You must give appropriate credit, provide a link to the license, 
	and indicate if changes were made. You may do so in any reasonable manner, but 
	not in any way that suggests the licensor endorses you or your use.

	NonCommercial — You may not use the material for commercial purposes.

	No additional restrictions — You may not apply legal terms or technological 
	measures that legally restrict others from doing anything the license permits.
	
	Notices:
	You do not have to comply with the license for elements of the material in the 
	public domain or where your use is permitted by an applicable exception or limitation.

	No warranties are given. The license may not give you all of the permissions necessary 
	for your intended use. For example, other rights such as publicity, privacy, or moral r
	ights may limit how you use the material.

  
  
*/


#ifndef SX70_ALPHA_RW_PCB_H 
#define SX70_ALPHA_RW_PCB_H 


//#include "Arduino.h"




/*		see camera-sx70.h for proper explanation of each port
		here we *ONLY* assign the value corresponding to the PCB
		since there are differences in older PCBs.
		these ports are for ALPHA REWORKED arduino pcbs only
		
*/

/*
const uint8_t PIN_S1 = 12; 						//INPUT red button 

const uint8_t PIN_S2 = A4;  					//INPUT this for Flash & dongle insertion detection

//#define S2  A4

const uint8_t PIN_S3 = 8;     					//INPUT S3: LOW = CLOSED
											//OPENS 26º-34º
											//Y-delay activated
											//Mirror ar rest
const uint8_t PIN_S5 = 7;     					//INPUT S5: LOW = CLOSED
											//OPENS:
											// Motor Braked	
											// Cam Stopped at 20°	
											// Electronic latch-ON	
											//(S1 may be opened)
const uint8_t PIN_S8 = A1;     					//S8: HIGH = CLOSED
const uint8_t PIN_S9 = A0;     					//S9: HIGH = CLOSED

const uint8_t Solenoid1 = 3;           		//OUTPUT 6V High Power
											//Shutter operation
const uint8_t Solenoid2 = 11;          		//OUTPUT 6V High Power
											//flash close aperture control

const uint8_t FFA = 4;						//OUTPUT flash fire
											//trigger flash


const uint8_t Motor = A2;					//OUTPUT Motor engagement
											//motor on and off according to S3 and S5

// NON-CAMERA ports

const uint8_t led1 = A3;					//VF LED1
const uint8_t led2 = 13;					//VF LED2
*/

// Input pin definition
const uint8_t PIN_S1 = 12;
const uint8_t PIN_S2 = 2;
const uint8_t PIN_S3 = 8;
// const uint8_t PIN_S4 = ;
const uint8_t PIN_S5 = 7;
const uint8_t PIN_S8 = A1;
const uint8_t PIN_S9 = A0;

// Output pin definition
const uint8_t PIN_SOL1 = 3;
const uint8_t PIN_SOL2 = 11;
const uint8_t PIN_FFA = 4;
const uint8_t PIN_MOTOR = A2;

const uint8_t PIN_LED1 = 13;
const uint8_t PIN_LED2 = A3;

//#define EE_ADD_PIC 13
//#define EE_ADD_ISO 20
//#define CLICKBTN_PULLUP HIGH

void io_init();


#endif

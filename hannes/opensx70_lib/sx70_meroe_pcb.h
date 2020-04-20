#ifndef SX70_MEROE_PCB_H 
  #define SX70_MEROE_PCB_H 
  //#include "Arduino.h"
  //#include "sx70_meroe_pcb.h"
  //#include "open_SX70.h"
  
  /*		see camera-sx70.h for proper explanation of each port
  		here we *ONLY* assign the value corresponding to the PCB
  		since there are differences in older PCBs.
  		these ports are for MEROE arduino pcbs only		
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
  const uint8_t PIN_S2 = A4;
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

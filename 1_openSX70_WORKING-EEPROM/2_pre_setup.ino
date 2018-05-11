#define ISDEBUG 0
//*************

//TEST ONLY SHUTTER ASSEMBLY
#define SHUTTER 1
//*************

//TEST ONLY BACK OF THE CAMERA
#define MOTOR 1
//*************

//LIGHTMETER ON BOARD?
#define LIGHTMETER 0
//*************

//CAMERA MODEL ALPHA OR MODEL2 (only one!!!)
#define ALPHA 1
#define MODEL2 0
//*************

// LIGHT METER STUFF BH1750*****************************************************************
#if LIGHTMETER

#include <Wire.h>
#include <BH1750.h>


BH1750 lightmeter;
//const byte luxMode = BH1750_CONTINUOUS_HIGH_RES_MODE;
const byte luxMode = BH1750_CONTINUOUS_HIGH_RES_MODE_2;
//const byte luxMode = BH1750_ONE_TIME_HIGH_RES_MODE_2;

// BH1750_CONTINUOUS_HIGH_RES_MODE
// BH1750_CONTINUOUS_HIGH_RES_MODE_2
// BH1750_CONTINUOUS_LOW_RES_MODE
// BH1750_ONE_TIME_HIGH_RES_MODE
// BH1750_ONE_TIME_HIGH_RES_MODE_2
// BH1750_ONE_TIME_LOW_RES_MODE
#endif

// ONE WIRE STUFF DS2408*****************************************************************

#include <EEPROM.h>

#include <DS2408.h>
//Based on DS2408/Arduino/lightLed/lightLed.ino

const int S2 = 2;  //this for Flash insertion detection
                      //this CLOSED when there is a FLASHBAR inserted

#define ONE_WIRE_BUS_PORT S2


//DS2408 ds(ONE_WIRE_BUS_PORT);
//Read_DS2408_PIO ds(ONE_WIRE_BUS_PORT);

//Devices devices;
//uint8_t device_count;

DS2408 ds(ONE_WIRE_BUS_PORT);

Devices devices;
uint8_t device_count = 0;

// DS2408*****************************************************************

//High speed PWM
const byte n = 224;  // for example, 71.111 kHz


const int S1 = 12;     //Red button SHUTTER RELEASE
                      //S1: LOW = CLOSED
const int S3 = 8;     //S3: LOW = CLOSED
const int S5 = 7;     //S5: LOW = CLOSED
const int S8 = A1;     //S8: HIGH = CLOSED
const int S9 = A0;     //S9: HIGH = CLOSED

//const int Solenoid1 = 5;           // 6V High Power

// in Aladdin :
const int Solenoid1 = 3;           // 6V High Power

const int Solenoid2 = 11;          // 6V High Power    




// in Aladdin :

const int FFA = 4;

//const int FFA = 3;


// ONE WIRE OUTPUT "PIN"s
const int DS_led = 6;
const int DS_pc_flash = 7;


                // MOTOR JUST TURNS ON OR OFF THE MOTOR
                // DEPENDS ON S3 AND S5 STATES PRIMARILY

const int Motor = A2;

/* THESE ARE THE INPUTS USED TO "READ" THE SHUTTER SPEED SELECTOR
const int switchPin1 = A0; // the number of the switch’s pin
const int switchPin2 = A1; // the number of the switch’s pin
const int switchPin3 = A2; // the number of the switch’s pin
const int switchPin4 = A3; // the number of the switch’s pin
*/

unsigned long DoubleExposureTimer = 0;

bool takePicture = false;

//int ActualSlot;                //selectorPOSITION is the position of the selector, we assign speeds with ShutterSpeed
                                //These are the actual speed in ms
                                //int ShutterSpeed[] = {0, 1000, 500, 250, 125, 66, 33, 16, 8, 0};
                                //inversed wheel

//int ShutterSpeed[] = {"B","T",4, 8, 16, 33, 66, 125, 250, 500, 1000, 1, 2, 3, 4, 5};

//enum positions_t {POS1 = -100, POS2, A100, A600, POSFLASH, POST, POSB};

/* THIS IS A600 POSITION FOR THE TIME BEING NO AUTO SLOT!
enum positions_t {A600 = -100, POSFLASH, POSFLASHF8, POST, POSB};

 TO CLARIFY:

 *  A600 = -100
 *  POSFLASH = -99
 *  POSFLASHF8 = -98
 *  POST = -97
 *  POSB = -96
 */

enum positions_t {POSFLASH = -100, POSFLASHF8, POST, POSB};
/*
 TO CLARIFY:

 *   POSFLASH = - 100
 *  POSFLASHF8 = -99
 *  POST = -98
 *  POSB = -97
*/

//int ShutterSpeed[] = { 40, 50, 56, 58, 62, 66, 84, 120, 180, 550, 1050, A100, A600, POSFLASH, POST, POSB };

//int ShutterSpeed[] = { 10, 15, 20, 40, 62, 66, 84, 120, 180, 550, 1050, A100, A600, POSFLASH, POST, POSB };

//int ShutterSpeed[] = { 10, 12, 15, 16, 18, 20, 35, 50, 66, 120, 180, A100, A600, POSFLASH, POST, POSB };

//int ShutterSpeed[] = { 9, 10, 12, 13, 15, 20, 25, 45, 66, 120, 180, A100, A600, POSFLASH, POST, POSB };

//int ShutterSpeed[] = { 11, 12, 13, 14, 18, 25, 45,90, 150, 300, 1240, A600, POSFLASH, POSFLASHF8, POST, POSB };


int ShutterSpeed[] = { 11, 12, 13, 14, 18, 25, 32, 45, 90, 150, 300, 1240, POSFLASH, POSFLASHF8, POST, POSB };

int shots = 0;

int pressTime = 0;     //intialize pressTime            

         // Lets define what is considered a longPress and a shortPress
         // shortPress is when you want to take a "regular" picture
         // it needs to be timed for debounce purposes, that is, you need a "solid" press to take a picture
  
const int shortPress = 100;
  
         //longPress is when you want to "something else", in my case delay the taking of the picture for x (10) seconds.
         //since 1000ms = 1 seconds, this is just a bit more than 1 second.

  const int  longPress = 1200;

// EEPROM STUFF
//EEPROM INITIALIZATION fixed positions
// 0 -->S
// 1 -->X
//10 --> pointer int eeAddress

int eeAddress;

//13 --> Picture counter int ActualPicture


//15-100 --> empty for future settings auto exposure etc...
//101 --> intial record

// record struct structure
// Picture sequence
// Type (manual, a100,a600, flash etc...)
// ShutterSpeed actual for auto reference/record
// lux measured
 
  struct Picture  {
  int StructPicture;
  byte PackPicture;
  byte StructType;
  int StructSpeed;
  uint16_t StructLux;
  };

  byte Pack = 1;
  
int ActualPicture;
byte CurrentPicture;
byte PictureType;
int eepromSpeed;
uint16_t lux;
//float lux;


  
//***************************************************************************************************************************************
//FUNCTION PROTOTYPES
byte Read_DS2408_PIO(int Slot);
byte Write_DS2408_PIO(byte port, bool ON);
int REDbutton(int button);
void motorON();
void motorOFF();
void shutterCLOSE();
void shutterOPEN();
void mirrorDOWN();
void mirrorUP();
void darkslideEJECT();
void Click();
void BeepTimerDelay();
void BlinkTimerDelay();
void LEDTimerDelay();
void Dongle (int DongleSlot);
void HighSpeedPWM ();
void Flash ();
void ShutterB();
void ShutterT();
void Ydelay ();
bool beep (bool state,int Pin);
void simpleBlink (int times);  
void eepromUpdate ();
void DongleFlash ();
void DongleFlashNormal ();
void eepromDump ();
void eepromDumpCSV ();

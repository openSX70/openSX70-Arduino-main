
//OPTION DEBUG INFORMATION ON SERIAL PORT (normal operation set to 0)

#define SIMPLEDEBUG 0  //SHOW DEBUG INFORMATION ON SERIAL MONITOR (9600)

//****************************************************************************************************************************

//****************************************************************************************************************************
#define LEDCOUNTER 0
// #define LEDCOUNTER 0    // easy way to disable the initial remaining pictures counter
//LEDCOUNTER 0 for auto, indicates dongleless auto mode 1= 100 2=600

//****************************************************************************************************************************

//OPTION FOR VF LEDS
//
#define VFled 1

//****************************************************************************************************************************


unsigned long counter;
unsigned long previousMillis;
//int ISO;
  //ISO for dongleless is going to be stored in EEPROM position 20.

  byte cISO;

//  const int OE_Pin = 9;  //PIN FOR CHIP ENABLE TSL23XT like TSL237T SMD

//****************************************************************************************************************************


#include <EEPROM.h>


// OneWire and DS2408 STUFF*****************************************************************
#include <DS2408.h>

//OPTION: the pin I connect S2 might be different on reworked or future boards. I want to connect to an ANALOG input so I can have A8-dongle support.

//const int S2 = 2;  //this for Flash insertion detection . //ALPHA!!

//
const int S2 = A4;  //this for Flash insertion detection MEROË!!!!!!!!!!


//const int S2 = A3;  //this for Flash insertion detection ALPHA REWORKED!!!!!!!!!!
//this CLOSED when there is a FLASHBAR inserted


#define ONE_WIRE_BUS_PORT S2
DS2408 ds(ONE_WIRE_BUS_PORT);
Device dongleDevice;
uint8_t device_count = 0;
//
uint8_t readDevice;

byte selector ;
bool switch1 ;
bool switch2 ;

// END OF OneWire and DS2408 STUFF*****************************************************************




// DS2408*****************************************************************

//High speed PWM
const byte n = 224;  // for example, 71.111 kHz

//SX70 to Arduino ports connections.

const int S1 = 12;     //Red button SHUTTER RELEASE

#if SONAR
const int S1F = 10;      //Pre-Focus on sonar
const int GTD = A5;     //SONAR Gone The Distance
#endif

//S1: Logic Alpha and previous LOW = CLOSED, SONAR HIGH = CLOSED ***NOW*** is set in S1Logic boolean 
const int S3 = 8;     //S3: LOW = CLOSED
const int S5 = 7;     //S5: LOW = CLOSED
const int S8 = A1;     //S8: HIGH = CLOSED
const int S9 = A0;     //S9: HIGH = CLOSED

const int Solenoid1 = 3;           // 6V High Power
const int Solenoid2 = 11;          // 6V High Power

const int FFA = 4;


// ONE WIRE OUTPUT "PIN"s
//const int DS_led = 6;
//const int DS_pc_flash = 7;


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

//****************************************************************************************************************************

//OPTION REGARDING SELECTOR WHEEL

enum positions_t {AUTO600 = -100, AUTO100, POST, POSB};
/*
  TO CLARIFY:

     AUTO600 = - 100
    AUTO100 = -99
    POST = -98
    POSB = -97
*/


int ShutterConstant = 9 ;

//OPTION
//int ShutterSpeed[] = { 9, 11, 13, 14, 18, 25, 32, 45, 53, 90, 150, 300, POSFLASH, POSFLASHF8, POST, POSB };
//int ShutterSpeed[] = { 9, 11, 13, 14, 18, 23, 30, 42, 50, 88, 148, 298, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing
//int ShutterSpeed[] =   { 1, 4,  6,  7,  11, 17, 22, 34, 42, 80, 140, 290, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing
//int ShutterSpeed[] = { 1, 4,  6,  7,  11, 17, 22, 34, 42, 80, 140, 290, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing
//int ShutterSpeed[] = { 6, 10, 12 , 14,  16, 24, 34, 44, 66, 82, 152, 292, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing
//////int ShutterSpeed[] = { 4, 8, 11 , 13,  15, 22, 34, 4, 26, 42, 112, 252, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing
//int ShutterSpeed[] = { 4, 8, 11 , 13,  15, 22, 34, 5, 27, 44, 112, 252, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing
/////int ShutterSpeed[] = { 15, 19, 22 , 24,  26, 33, 45, 66, 82, 135, 162, 300, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing
//int ShutterSpeed[] =   { 15, 16,  17,  18,  19, 20, 21, 22, 23, 24, 25, 26, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing

int ShutterSpeed[] = { 17, 20, 23 , 25,  30, 35, 45, 55, 68, 102, 166, 302, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing



//OPTION line above are the wheel "raw" speeds (have to keep in mind smaller time = smaller aperture)
// this are the "SLOTS":
//int ShutterSpeed[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F };
//int ShutterSpeed[] = { EV17, EV16, EV15, EV14, EV13, EV12, EV11.5, EV11, EV10.5, EV10, EV9, EV8, AUTO600, AUTO100, T, B };
// to change the speed in the slot position just change the number corresponding.


//flashDelay is the time it takes to fire the flash (even if none is connected) when slow shutterspeeds
//int flashDelay = 39; //this was for flash as port 7 on the DS2408, old uDongles.

int flashDelay = 1; //new flash "system"

//this speed and SLOWER will trigger flash

int shots = 0;

//****************************************************************************************************************************

//OPTION RED Button timing variables
/////int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int debounce = 30;          // ms debounce period to prevent flickering when pressing or releasing the button
////int DCgap = 250;            // max ms between clicks for a double click event
int DCgap = 250;            // max ms between clicks for a double click event

int holdTime = 350;        // ms hold period: how long to wait for press+hold event
//int longHoldEventPast = 5000;
//int longHoldTime = 5000; // ms long hold period: how long to wait for press+hold event



// Lets define what is considered a longPress and a shortPress
// shortPress is when you want to take a "regular" picture
// it needs to be timed for debounce purposes, that is, you need a "solid" press to take a picture

//Some people press the red button until it ejects, this is NOT happening with openSX70 as it is right now.
//I might try to "fix" this in the future, maybe by making the self timer activation by a double click. It is not a priority right now.

//Cristina is killing me with this, she just presses too short or too long (going unadvertedly to self timer) so I am going to change the values a bit.

//****************************************************************************************************************************

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

struct Picture
{
  int StructPicture;         //total count of pictures since init
  byte StructPackPicture;          //pic count within this pack
  byte StructType;           //picture type

  // PictureType = 0 ---> MANUAL
  // PictureType = 1 ---> A100
  // PictureType = 2 ---> FLASH DONGLELESS
  // PictureType = 4 ---> FLASH F8 DONGLE
  // PictureType = 6 ---> A600
  // PictureType = +10 ---> MIRROR DELAY
  // PictureType = +100 ---> MULTIPLE EXPOSURE
  // PictureType = +200 ---> TIMER DELAY

  byte StructSpeed;           //shutter speed
  int Counter;
  //int StructLightVlow ;      //photodiode read LOW
  //int StructLightVhigh ;     //photodiode read HIGH
};


byte Pack = 1;

int ActualPicture;
byte CurrentPicturePack;
byte PictureType;
byte eepromSpeed;
int Counter;    //Photodiode stuff
//int sensorValueHIGH;   //Photodiode stuff

//***************************************************************************************************************************************
//FUNCTION PROTOTYPES
byte Read_DS2408_PIO(int Slot);
void Write_DS2408_PIO(byte port, bool ON);
int REDbutton(int button);
void motorON();
void motorOFF();
void shutterCLOSE();
void shutterOPEN();
void mirrorDOWN();
void mirrorUP();
void darkslideEJECT();
void Click(int Exposure);
void BeepTimerDelay();
void BlinkTimerDelay();
void LEDTimerDelay();
void Dongle (int DongleSlot);
void HighSpeedPWM ();
void FlashBAR();
void ShutterB();
void ShutterT();
void Ydelay ();
//bool beep (bool state,int Pin);
void simpleBlink (int times, int led);
void eepromUpdate ();
void DongleFlashF8();
void DongleFlashNormal ();
void eepromDump ();
void eepromDumpCSV ();
void startCounter();
void AutoExposure();
int checkButton();
void startCounterCalibration();
void ManualExposure();
void initializeDS2408();
//int frequencyCounter(int ISO);
int nearest(int x, int myArray[], int elements, bool sorted);
int PredictedExposure();
int ISO();
bool DebouncedRead(int pin);
void tsl237_init();
void finish();
//****************************************************************************************************************************

//DEBUG OR NOT
#define ISDEBUG 1
//*************

//TEST ONLY SHUTTER ASSEMBLY
#define SHUTTER 1
//*************

//TEST ONLY BACK OF THE CAMERA
#define MOTOR 0
//*************


// ONE WIRE STUFF DS2408*****************************************************************

#include <DS2408.h>
//Based on DS2408/Arduino/lightLed/lightLed.ino
const int S2 = 2;  //this for Flash insertion detection
                      //this CLOSED when there is a FLASHBAR inserted

#define ONE_WIRE_BUS_PORT S2

DS2408 ds(ONE_WIRE_BUS_PORT);

Devices devices;
uint8_t device_count;

// DS2408*****************************************************************

//High speed PWM
const byte n = 224;  // for example, 71.111 kHz


const int S1 = 12;     //Red button SHUTTER RELEASE
                      //S1: LOW = CLOSED
const int S3 = 8;     //S3: LOW = CLOSED
const int S5 = 7;     //S5: LOW = CLOSED
const int S8 = A1;     //S8: HIGH = CLOSED
const int S9 = A0;     //S9: HIGH = CLOSED

const int Solenoid1 = 3;           // 6V High Power
const int Solenoid2 = 11;          // 6V High Power    

const int FFA = 4;




// ONE WIRE OUTPUT "PIN"s
const int DS_led = 6;
const int DS_pc_flash = 7;


                // MOTOR JUST TURNS ON OR OFF THE MOTOR
                // DEPENDS ON S3 AND S5 STATES PRIMARILY
const int Motor = 13;

/* THESE ARE THE INPUTS USED TO "READ" THE SHUTTER SPEED SELECTOR
const int switchPin1 = A0; // the number of the switch’s pin
const int switchPin2 = A1; // the number of the switch’s pin
const int switchPin3 = A2; // the number of the switch’s pin
const int switchPin4 = A3; // the number of the switch’s pin
*/


bool takePicture = false;

int ActualSlot;                //selectorPOSITION is the position of the selector, we assign speeds with ShutterSpeed
                                //These are the actual speed in ms
                                //int ShutterSpeed[] = {0, 1000, 500, 250, 125, 66, 33, 16, 8, 0};
                                //inversed wheel

//int ShutterSpeed[] = {"B","T",4, 8, 16, 33, 66, 125, 250, 500, 1000, 1, 2, 3, 4, 5};

enum positions_t {POS1 = -100, POS2, POS3, POS4, POS5, POSB, POST};

int ShutterSpeed[] = {POSB, POST, 4, 8, 16, 33, 66, 125, 250, 500, 1000, POS1, POS2, POS3, POS4, POS5 };



int shots = 0;

int pressTime = 0;     //intialize pressTime            

         // Lets define what is considered a longPress and a shortPress
         // shortPress is when you want to take a "regular" picture
         // it needs to be timed for debounce purposes, that is, you need a "solid" press to take a picture
  
const int shortPress = 150;
  
         //longPress is when you want to "something else", in my case delay the taking of the picture for x (10) seconds.
         //since 1000ms = 1 seconds, this is just a bit more than 1 second.

  const int  longPress = 1200;
//***************************************************************************************************************************************
//FUNCTION PROTOTYPES
byte DS2408(int Slot);
int REDbutton(int button);
void motorON();
void motorOFF();
void shutterCLOSE();
void shutterOPEN();
void mirrorDOWN();
void mirrorUP();
void darkslideEJECT();
void Click();
void timerDelay();
void Dongle (int DongleSlot);
void HighSpeedPWM ();
void Flash ();
void ShutterB();
void ShutterT();

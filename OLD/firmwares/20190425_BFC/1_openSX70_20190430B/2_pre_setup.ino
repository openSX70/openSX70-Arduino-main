
//****************************************************************************************************************************
//OPTION for origamiV1 dongles

//
#define origamiV1 0 //this is for "normal" uDongles

//#define origamiV1 1 //this is for "origamiV1" dongles, will be fixed in newer origamis.

//suport for origami V1 boards.

//****************************************************************************************************************************

//OPTION DEBUG INFORMATION ON SERIAL PORT (normal operation set to 0)

#define SIMPLEDEBUG 0  //SHOW DEBUG INFORMATION ON SERIAL MONITOR (9600)
//*************

//****************************************************************************************************************************
#define LEDCOUNTER 1
// #define LEDCOUNTER 0    // easy way to disable the initial remaining pictures counter


//****************************************************************************************************************************
//OPTION LIGHTMETER ON BOARD?
// if you define LIGHTMETER 0 then auto options and dongleless DO NOTHING

#define LIGHTMETER 1      

int output_compare ;        // How many pulses before triggering interrupt
                                  //NOW this value is for dongleless auto
//OPTION MAGIC NUMBERS FOR TSL235
int A600 = 205; 
int A100= 485;
                                  
unsigned long counter;

//****************************************************************************************************************************

//OPTION POWER DOWN 
//Alpha or Model 2 might different values

const byte PowerDownDelay = 15; //time it takes to be fully closed
const byte PowerDown = 225; //max 255 = full power/POWERUP mode
//*************
//This is experimental, actually Model2's don't seem to work, or mine is a lemon

//****************************************************************************************************************************

//OPTION LED: Alpha Boards have two leds visible in a sonar-type VF. 
//Might be connected to different arduino ports on reworked PCBs normally A3 or A5

const int led1 = 13;
const int led2 = A3;

//****************************************************************************************************************************

#include <EEPROM.h>


// OneWire and DS2408 STUFF*****************************************************************
#include <DS2408.h>

//OPTION: the pin I connect S2 might be different on reworked or future boards. I want to connect to an ANALOG input so I can have A8-dongle support.

const int S2 = 2;  //this for Flash insertion detection
                      //this CLOSED when there is a FLASHBAR inserted

//const int S2 = A4;  //this for Flash insertion detection for reworked board


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
                      //S1: LOW = CLOSED
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

 *   AUTO600 = - 100
 *  AUTO100 = -99
 *  POST = -98
 *  POSB = -97
*/


int ShutterConstant = 9;

//OPTION
int ShutterSpeed[] = { 9, 11, 13, 14, 18, 23, 30, 42, 50, 88, 148, 298, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing


//OPTION line above are the wheel "raw" speeds (have to keep in mind smaller time = smaller aperture)
// this are the "SLOTS":
//int ShutterSpeed[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F };
//int ShutterSpeed[] = { EV17, EV16, EV15, EV14, EV13, EV12, EV11.5, EV11, EV10.5, EV10, EV9, EV8, AUTO600, AUTO100, T, B };
// to change the speed in the slot position just change the number corresponding.

int FastestFlashSpeed = 25;

//this speed and SLOWER will trigger flash

int shots = 0;

//****************************************************************************************************************************

//OPTION RED Button timing variables
int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 250;            // max ms between clicks for a double click event
int holdTime = 350;        // ms hold period: how long to wait for press+hold event
       

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
    int StructLightVlow ;      //photodiode read LOW
    int StructLightVhigh ;     //photodiode read HIGH
    };


      byte Pack = 1;
  
      int ActualPicture;
      byte CurrentPicturePack;
      byte PictureType;
      byte eepromSpeed;
      int Counter;    //Photodiode stuff
      int sensorValueHIGH;   //Photodiode stuff

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
void simpleBlink (int times);  
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
unsigned int frequencyCounter();
//****************************************************************************************************************************

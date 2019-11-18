//MEROË TCS3200


//****************************************************************************************************************************
//OPTION for ALPHA (0) or SONAR (1)

//
#define SONAR 0 //this is for "normal" uDongles

#define SONARPCB 1//SONAR PCBs on Alpha camera

//****************************************************************************************************************************

//OPTION S1 closed logic
#if SONARPCB
boolean S1Logic = HIGH; //SONAR
#endif

#if !SONARPCB
boolean S1Logic = LOW; //MEROE ALPHA and previous
#endif

//****************************************************************************************************************************
//OPTION for origamiV1 dongles

//
#define origamiV1 0 //this is for "normal" uDongles

//#define origamiV1 1 //this is for "origamiV1" dongles, will be fixed in newer origamis.

//suport for origami V1 boards.

//****************************************************************************************************************************
//OPTION LIGHTMETER ON BOARD?
// if you define LIGHTMETER 0 then auto options and dongleless DO NOTHING

#define LIGHTMETER 1
#define TS3200 0
#define TLS237T 1

int output_compare ;

#if TS3200
//const int S0_Pin = 3;
const int S1_Pin = 2;                   //
//const int S2_Pin = A0;
const int S3_Pin = 6;
const int OE_Pin = 9;

//  pinMode(S1_Pin, OUTPUT);    
//  pinMode(S3_Pin, OUTPUT);    
//  pinMode(OE_Pin, OUTPUT);    

  //S2 & S0 should be high can be modified via jumper in PCB 
//  digitalWrite(S1_Pin, HIGH); //scaling LOW = 20% HIGH = 100%
//  digitalWrite(S3_Pin, LOW); //filter LOW = clear HIGH = green
//  digitalWrite(OE_Pin, LOW); //LOW = enabled

//TCS3200 100% SETTINGS
//const int OE_Pin = 9;
/////int A600 = 450;
int A600 = 420; //?????????????

//int A100 = 1000;
int A100 = 980; //??????????????
#endif

#if TLS237T
        // How many pulses before triggering interrupt
//NOW this value is for dongleless auto
//OPTION MAGIC NUMBERS FOR TSL235
//int A600 = 205;
//int A600 = 225;
//int A100 = 495;
//int A100 = 515;
//MEROË TSL237T 2X SETTINGS
/////int A600 = 450;
int A600 = 420; //lowered 20190906

//int A100 = 1000;
int A100 = 980;
#endif

//OPTION POWER DOWN
//Alpha or Model 2 might different values

const byte PowerDownDelay = 15; //time it takes to be fully closed
const byte PowerDown = 195; //max 255 = full power/POWERUP mode
//*************
//This is experimental, actually Model2's don't seem to work, or mine is a lemon

//****************************************************************************************************************************

//OPTION LED: Alpha Boards have two leds visible in a sonar-type VF.
//Might be connected to different arduino ports on reworked PCBs normally A3 or A5

//const int led1 = 13;
//const int led2 = A3;

//MEROË led colors are reversed?

const int led2 = 13;
const int led1 = A3;


//****************************************************************************************************************************

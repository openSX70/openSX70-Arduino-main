#ifndef settings_h
  #define settings_h
  #include "Arduino.h"
  //----------------DEBUG OPTIONS----------------------------------------
  #define DEBUG 0
  #define SIMPLEDEBUG 0     //Debug of opensx70 functions
  #define ADVANCEDEBUG 0    //More detailed view of OpenSX70 functions
  #define BASICDEBUG 0      //Debug of Basic Camera Functions
  #define MXDEBUG 0         //Multiple Exposure Debug
  #define LMDEBUG 0      //Lightmeter Debug
  #define LMHELPERDEBUG 0   //Lightmeter Debug
  #define ROTARYDEBUG 0     //Rotaryswitch on Dongle Debug
  #define STATEDEBUG 0      //Debug state transitions
  #define FOCUSDEBUG 0      //Focus Debug on Sonar
  #define FFDEBUG 0         //AutoexposureFillFlash Debug
  #define STATUSDEBUG 0

  //----------------EXPERIMENTAL OPTIONS---------------------------------
  
  #define APERTURE_PRIORITY 0      //1 Enables SOL2 engage on picture taking for experimental aperture priority mode

  //----------------CAMERA PCB OPTIONS SELECTION-------------------------
  //      Sensor Selection
  #define TCS3200 1                //Meroe, Land
  #define TSL237T 0                //Edwin
  #define TSL235R 0                //Alpha 2
  //      Board Version
  #define MEROE_PCB 1
  #define LAND_PCB 0
  #define SONAR_PCB 0
  #define SONAR_UNI_PCB 0
  #define EDWIN_PCB 0
  #define ALPHA2_PCB 0
  #define ECM_PCB 0
  //      Camera Options
  #define S1Logic LOW             //LOW for Alpha shutters and HIGH for Sonar shutters
  #define ALPHA 1                  //1 if ALPHA camera functions
  #define SONAR 0                  //1 for Sonar camera functions   
  //      Dongle Options 
  #define FLIP_ORDER 0             //1 to set dongle selector order to "a600 a100 T B" rather than "T B a600 a100". This is for older dongle revisions.
  #define AFTER_EIGHT 0
  #define ATTINY 1
  #define UDONGLE 1
  #define ORIGAMI 0
  #define ORIGAMIV1 0
  #define GREEN 6
  #define RED 7
  //      Feature toggles
  #define DOUBLECLICK 0            //1 Enables double clicking the shutter button without a dongle to start 10s self timer
  #define SHUTTERDARKSLIDE 0       //1 Enables feature to not eject dark slide until shutter button is pressed
  #define COUNTER_BLINK 0          //1 Switches dongle LED functionality from displaying ISO to displaying the number of shots remaining in your film pack
  #define TIMER_MIRROR_UP 0        //1 Enables mirror up feature for self timer modes
  #define EIGHT_SHOT_PACK 1        //1 Makes all counter-based functions work based on an 8 shot pack rather than 10
  #define LIGHMETER_HELPER 1       //1 Enables viewfinder light meter helper
  #define EJECT_AFTER_DEPRESSING 1 //1 Enables the user to hold the shutter button to prevent photo ejection
  #define DONGLE_FREE_ISO_CHANGE 1 //1 Enables holding the shutter button on boot to swap ISO. Blue blink on PCB means 600, red means SX-70
  
  //----------------END CAMERA PCB OPTIONS SELECTION------------------------

  //----------------ISO VALUES VALUES---------------------------------------
  #define ISO_600 640
  #define ISO_SX70 125
  #define DEFAULT_ISO ISO_SX70
  //---------------END ISO VALUES--------------------------------------------

  //---------------MAGIC NUMBERS---------------------------------------------
  #if TCS3200
    //#define 585
    //#define 210
    #define A100 425
    #define A600 150
  #endif
  #if TSL237T     
    #define A100 950
    #define A600 420
  #endif
  #if TSL235R
    #define A100 485
    #define A600 225
  #endif
  //---------------END MAGIC NUMBERS-----------------------------------------

  //---------------Flashbar and Dongle Flash---------------------------------
  #define Flashbar_Change 1
  #define Flash_Capture_Delay 4
  #define Flash_Max_Time 63
  #define Flash_Min_Time 47
  #define Dongle_Flash_Limit 4
  //---------------End Flash settings----------------------------------------

  //---------------Shutter Settings------------------------------------------
  #define SELECTOR_LIMIT_VARIANCE 6
  //---------------End Shutter Settings--------------------------------------

  //---------------METER SETTINGS--------------------------------------------
  #define METER_INTERVAL 100 // Sets how long each meter measurement sample is taken in ms
  #define METER_AUTO_WARNING 100 // If predicted ms is over this value, warning LED will shine in auto mode
  #define METER_PREDICTION_OFFSET 20 // in ms. This gets added to the prediction. At f8 I noticed all meter predictions were around 20ms off
  #define METER_SLOPE_HANDICAP 0 // Not used currently. Used to increase/decrease the slope of the prediction.
  //---------------END METER SETTINGS----------------------------------------

  //---------------MULTIPLE EXPOSURES MODE SETTINGS--------------------------
  #define MULTIPLE_EXPOSURES_TIMEOUT_ENABLED 1 // 1 -> MX mode will finish after timeout. 0 -> No time limit for MX mode.
  #define MULTIPLE_EXPOSURES_TIMEOUT 120000 // Max time that MX mode can run for after the first exposure (in milliseconds)
  //---------------END MULTIPLE EXPOSURES MODE SETTINGS----------------------

  #define EE_ADD_CUR   4    //-> CurrentPicture --> 1-8 "Pack" picture counter
  #define EE_ADD_ADD   30    //-> eeAddress Where to write next
  #define EE_ADD_PIC   13    //-> ActualPicture --> Counter from begining
  #define EE_ADD_ISO   20   //-> camera current ISO (dongleless) 

  enum positions_t {POST = -100, POSB, AUTO600, AUTO100};//ANALOGUEWORKS AUTO 600BW

  extern int FD100;
  extern int FD600;
  extern int ShutterConstant;
  extern int ShutterSpeed[]; //reduced speeds from 25 (slot5) to compensate flash firing
  extern int flashDelay; //new flash "system"
  extern int MeterRange[];
  extern int MaxRange[];
  extern int MinRange[];
  extern int ShutterVariance[];
  extern int mxshots;
  extern byte lightmeterHelper;
  extern const uint8_t YDelay;
  extern const byte PowerDownDelay;
  extern const byte PowerDown; //max 255 = full power/POWERUP mode
  #define DEBOUNCECOUNT 5 
  extern int currentPicture;
  extern int prevDongle;
#endif

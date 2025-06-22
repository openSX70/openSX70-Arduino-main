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
  //      Camera Options
  //May go crazy and take all of these out.
  //There is really no difference between an alpha and a sonar camera other than S1F as far as logic is concerned.
  //End user will probably never even feel a difference.
  #define S1Logic HIGH
  //      Dongle Options 
  #define GREEN 6
  #define RED 7
  //      Feature toggles
  #define DOUBLECLICK 0            //1 Enables double clicking the shutter button without a dongle to start 10s self timer
  #define SHUTTERDARKSLIDE 0       //1 Enables feature to not eject dark slide until shutter button is pressed
  #define TIMER_MIRROR_UP 1        //1 Enables mirror up feature for self timer modes
  #define EIGHT_SHOT_PACK 1        //1 Makes all counter-based functions work based on an 8 shot pack rather than 10
  #define LIGHMETER_HELPER 1       //1 Enables viewfinder light meter helper
  #define EJECT_AFTER_DEPRESSING 1 //1 Enables the user to hold the shutter button to prevent photo ejection
  //----------------END CAMERA PCB OPTIONS SELECTION------------------------

  //----------------DONGLE SWITCH FEATURE SELECTION-------------------------
  // 1 and 2 values assign features to switch 1 and 2, 0 means unused.
  // Example values:
  // #define MEXP_MODE 1   : MEXP_MODE on switch 1
  // #define SELF_TIMER 2  : SELF_TIMER on switch 2
  // #define {whatever} 0  : No switch assigned. 
  // DO NOT ASSIGN MULTIPLE THINGS TO THE SAME VALUE (except 0).
  // DOING SO WILL BREAK THINGS. YOU CANNOT HAVE MULTIPLE FUNCTIONS ASSIGNED TO THE SAME SWITCH.
  // When I have a configurator style dongle set up I will be doing validation on that side.
  // Until then, YOU will need to validate that you are not overloading a switch.

  #define MEXP_MODE 1
  #define SELF_TIMER 2

  //----------------END DONGLE SWITCH FEATURE SELECTION---------------------

  //----------------ISO VALUES VALUES---------------------------------------
  #define ISO_600 640
  #define ISO_SX70 125
  #define DEFAULT_ISO ISO_600
  //---------------END ISO VALUES--------------------------------------------

  //---------------MAGIC NUMBERS---------------------------------------------
  #define A100 1023
  #define A600 255
  
  //---------------END MAGIC NUMBERS-----------------------------------------

  //---------------Flashbar and Dongle Flash---------------------------------
  #define Flashbar_Change 1
  #define Flash_Capture_Delay 4
  #define Flash_Max_Time 56
  #define Flash_Capture_Max_Time 12
  #define Dongle_Flash_Limit 4
  //---------------End Flash settings----------------------------------------

  //---------------Shutter Settings------------------------------------------
  #define SELECTOR_LIMIT_VARIANCE 6
  //---------------End Shutter Settings--------------------------------------

  //---------------METER SETTINGS--------------------------------------------
  #define AUTO_TIMEOUT 15000 //Sets the maximum time an auto exposure can take place. Required due to new meter design.
  #define METER_INTERVAL 100 // Sets how long each meter measurement sample is taken in ms
  #define METER_AUTO_WARNING 100 // If predicted ms is over this value, warning LED will shine in auto mode
  #define METER_PREDICTION_OFFSET 20 // in ms. This gets added to the prediction. At f8 I noticed all meter predictions were around 20ms off
  #define METER_SLOPE_HANDICAP 0 // Not used currently. Used to increase/decrease the slope of the prediction.
  #define METER_RESET_DELAY 2 // Time required for light meter capacitor to fully reset
  //---------------END METER SETTINGS----------------------------------------

  //---------------MULTIPLE EXPOSURES MODE SETTINGS--------------------------
  #define MULTIPLE_EXPOSURES_TIMEOUT_ENABLED 1 // 1 -> MX mode will finish after timeout. 0 -> No time limit for MX mode.
  #define MULTIPLE_EXPOSURES_TIMEOUT 120000 // Max time that MX mode can run for after the first exposure (in milliseconds)
  //---------------END MULTIPLE EXPOSURES MODE SETTINGS----------------------

  #define EE_ADD_ISO   20   //-> camera current ISO (dongleless) 

  enum positions_t {POST = -100, POSB, AUTO600, AUTO100};//ANALOGUEWORKS AUTO 600BW

  extern int FD100;
  extern int FF100;
  extern int FD600;
  extern int FF600;
  extern int ShutterSpeed[]; //reduced speeds from 25 (slot5) to compensate flash firing
  extern int flashDelay; //new flash "system"
  extern int MaxRange[];
  extern int MinRange[];
  extern int ShutterVariance[];
  extern int mxshots;
  extern byte lightmeterHelper;
  extern const uint8_t YDelay;
  extern const byte PowerDownDelay;
  extern const byte PowerDown; //max 255 = full power/POWERUP mode
  #define DEBOUNCECOUNT 5 
  extern int prevDongle;
#endif
